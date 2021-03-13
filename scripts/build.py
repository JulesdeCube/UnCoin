from pathlib import Path
from datetime import datetime
import os
import sys
import re

if not 2 <= len(sys.argv) <= 3:
    print('Usage: build.py [[-d] target]')
    exit(1) 

def apply(func, *iterables):
  for args in zip(*iterables):
    func(*args)


def debug(variable):
    value = eval(variable)
    s = ''
    if isinstance(value, list):
        s = '[' + ',' .join(map(str, value)) + ']'
    if isinstance(value, dict):
        s = '{\n'
        for k, v in value.items():
            s += f'  {k}: '
            s += '\n  '.join(str(v).splitlines())
            s += '\n'
        s += '\n}'
    else:
        s = str(value)
    print(variable, '=', s)

src_dir = Path('src').resolve()
obj_dir = Path('obj').resolve()
build_dir = Path('build').resolve()
target = sys.argv[-1]

bin = (build_dir / target).resolve()
main = (Path.with_suffix(src_dir / target / target, '.c')).resolve()

objs = []
deps = {}

cc = 'gcc'
cflag = f'-I {src_dir} -Wall -Wextra -Werror -std=c99 -pedantic -export-dynamic -lm'
if '-d' in sys.argv:
    cflag += ' -g'

regex = re.compile('^#include "([^"]*)"$')

class info:
    def __init__(self, path):
        self.of = set()

        self.date = datetime.min
        if path.exists():
            self.date = datetime.fromtimestamp(int(path.stat().st_mtime))

        self.deps_date = self.date

    @staticmethod
    def get_deps_date(path):
        return deps[path].deps_date

    def calculate(self):
        self.deps_date = max([self.date] + list(map(self.get_deps_date, self.of)))

    @property
    def need_build(self):
        return self.date < self.deps_date

    @staticmethod
    def __date(date):
        if date == datetime.min:
            return 'min'
        if date == datetime.max:
            return 'max'
        return str(date)

    def __str__(self):
        return \
            '{\n' + \
            f'  date: {self.__date(self.date)};\n' + \
            f'  deps_date: {self.__date(self.deps_date)};\n' + \
            '  of: ' + ' '.join(map(str,self.of)) + '\n' + \
            '}'
            


def add(path):
    if path in deps:
        return False
    else:
        deps[path] = info(path)
        return True
    

def link(src, dep):
    add(src)
    deps[src].of.add(dep)


def get_deps(file):
    deps = []

    for match in map(regex.match, file):
        if not match:
            continue

        deps.append(match.group(1))

    return deps


def expend(src, dst):
    start = dst[0]

    if start == '.':
        path = src.parent / dst
    elif start == '/':
        path = src_dir / dst
    else:
        path = src.parent / dst
        if not path.exists():
            path = src_dir / dst
          
    if not path.exists():
        exit(f'ERROR: can\'t find "{dst}" from "{path}"')

    path = path.resolve()

    return path


def get_obj(path):
    src = path.relative_to(src_dir)
    obj = obj_dir / src.with_suffix('.o')
    obj.resolve()
    return obj


def parse(path):
    with open(path, 'r', encoding='utf8') as file:
        for dep in get_deps(file):
            dep = expend(path, dep)
            proccess(dep)
            link(path, dep)


def proccess(path):
    # not the first time we process it
    if not add(path):
        return

    parse(path)
    deps[path].calculate()

    if path.suffix == '.h':
        proccess(path.with_suffix('.c'))
    
    
    if path.suffix == '.c':
        obj = get_obj(path)
        link(obj, path)
        link(bin, obj)
        deps[obj].calculate()

    
def run_command(command):
    print(' '.join(command))
    result = os.system(' '.join(command))
    if result != 0:
        exit(result)


def gcc(path, option = '-c'):
    path.parent.mkdir(parents=True, exist_ok=True)
    run_command([cc, option, ' '.join(map(str, deps[path].of)), '-o', str(path) , cflag])


def need_build(path):
    return deps[path].need_build


proccess(main)
bin_info = deps[bin]
bin_info.calculate()

if bin_info.need_build:
    apply(gcc, filter(need_build, bin_info.of))
    gcc(bin, option='')
else:
    print('Up to date')