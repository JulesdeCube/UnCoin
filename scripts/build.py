from pathlib import Path
from datetime import datetime
import os
import sys
import re

# if there is no arguement print the usages
if not 2 <= len(sys.argv) <= 3:
    print('Usage: build.py [[-d] target]')
    exit(1)


def apply(func, *iterables):
    """ Apply function by getting one element of each elements of iterables. """
    for args in zip(*iterables):
        func(*args)


# Directory of all the source files
src_dir = Path('src').resolve()
# Object directory
obj_dir = Path('obj').resolve()
# Output directory for the binnary files
build_dir = Path('build').resolve()
# Name of the binary/project ot build (it's the last arguements)
target = sys.argv[-1]

# Binnary path files
bin = (build_dir / target).resolve()
# Main file entry
main = (Path.with_suffix(src_dir / target / target, '.c')).resolve()


# Compiler command
cc = 'gcc'
# Compiler flags
cflag = f'-I {src_dir} -Wall -Wextra -Werror -std=c99 -pedantic -export-dynamic -lm'
# if the script is call with a debug flag add debug flag to the cflag
if '-d' in sys.argv:
    cflag += ' -g'

# Regex for the includes
regex = re.compile('^#include "([^"]*)"$')


# Info of each files
infos = {}


def get_deps_date(path):
    """ The depencies max date of the file. """
    return infos[path].deps_date


def is_out_dated(path):
    """ If the file is out dated """
    return infos[path].out_dated


class Info:
    """ Basics info of a file. """

    def __init__(self, path):
        # set of the path of dependences
        self.deps = set()

        # the last modification date of the file (min if not exist)
        self.date = datetime.min
        # if the path exist get it's last modification dates
        if path.exists():
            self.date = datetime.fromtimestamp(int(path.stat().st_mtime))

        # last depedence modification date (can't be less than it's self)
        self.deps_date = self.date

    def update(self):
        """ Update the deps_date atribute. """
        # the max bwtween it's self and it's dependy dependities dates
        self.deps_date = max(
            [self.date] +
            list(map(get_deps_date, self.deps))
        )

    @property
    def out_dated(self):
        """ If the file out dated """
        return self.date < self.deps_date


def add(path):
    """ Add the file into the infos and return if we added or not. """
    # If file is already init return false
    if path in infos:
        return False
    # If it's not inside init it and return true
    else:
        infos[path] = Info(path)
        return True


def link(src, dep):
    """ Mark "dep" as a Dependence of the "src". """
    add(src)
    infos[src].deps.add(dep)


def get_deps(file):
    """ Get the list of the depedency of the file. """
    # deps list
    deps = []

    # get each match for each line
    for match in map(regex.match, file):
        # if there is a match
        if match:
            # add it to the dependecy
            deps.append(match.group(1))

    # retur the deps list (string)
    return deps


def expend(src, dst):
    # get the prefix
    start = dst[0]

    # if it is a relative path
    if start == '.':
        # calcule path relative to the source file
        path = src.parent / dst
    # if it's a absolute path
    elif start == '/':
        # use the root source dir as reference
        path = src_dir / dst
    # if no indication
    else:
        # test each of the possible origins
        path = src.parent / dst  # relative
        if not path.exists():
            path = src_dir / dst  # absolute

    # if the path didn't exist return an error
    if not path.exists():
        exit(f'ERROR: can\'t find "{dst}" from "{path}"')

    # normalize path to not have the dots paths
    path = path.resolve()

    # return the expended path
    return path


def is_source_file(path):
    """ return if the file is a source files """
    return path.suffix in ['.c', '.h']


def get_obj(path):
    """ Get the object file path base on the source file. """
    # get the path relative to the src path
    src = path.relative_to(src_dir)
    # create the object path by using the object dirrectory and replace the
    # suffix
    obj = obj_dir / src.with_suffix('.o')
    # normalize the path
    obj.resolve()
    # return it
    return obj


def parse_source(path):
    """ Add all the dependecy by getting the depency of the source file """
    with open(path, 'r', encoding='utf8') as file:
        for dep in get_deps(file):
            dep = expend(path, dep)
            proccess(dep)
            link(path, dep)


def proccess_module(path):
    """ Get the dependcy of the given modules """
    module = path.parent
    # if it's not a module do nothing
    if not module.exists():
        return
    # include all the public and private source
    for access in ['public', 'private']:
        # get the folter
        folder = module / access
        # if the folder exist
        if folder.exists():
            # procces each source files
            for dep in filter(is_source_file, folder.iterdir()):
                proccess(dep)
                link(path, dep)

    infos[path].update()


def proccess(path):
    """
    Get reclusively all the dependency of the path and compute it's dependency
    date.
    """
    # proccess one time each path
    # don't proccess not existing path
    if add(path) and path.exists():
        # get and proccess each depedency of the file
        parse_source(path)
        # update the dependency dates
        infos[path].update()

        # back propagate the header files

        # if it is a header get the corresponding source file
        if path.suffix == '.h':
            # proccess basics include
            proccess(path.with_suffix('.c'))
            # proccess module
            proccess_module(path)

        # if it is a source file add the object and linked it to the binnary
        if path.suffix == '.c':
            # get object path
            obj = get_obj(path)
            # link the source to the object
            link(obj, path)
            # link the object to the binnaty
            link(bin, obj)
            # update the object infos
            infos[obj].update()


def run_command(command):
    """ Print and run a command. """
    # show the command
    print(' '.join(command))
    # execute the command
    result = os.system(' '.join(command))
    # if there is an error return it
    if result != 0:
        exit(result)


def gcc(path, option='-c'):
    """ Compile the source file base on the option. """
    # create output dirrectory if not exist
    path.parent.mkdir(parents=True, exist_ok=True)

    # the current dir
    cwd = os.getcwd()

    def get_path(path):
        """ Shotten the path and covert it to string. """
        return str(path.relative_to(cwd))

    # the the gcc command with the corresponding option
    run_command([
        cc,
        option, ' '.join(map(get_path, infos[path].deps)),
        '-o', get_path(path),
        cflag
    ])


# build the depencies graph
proccess(main)
# get info the the binnary
bin_info = infos[bin]
# upate to get it's dependecies date
bin_info.update()

# if the binnary is up to date do noting
if bin_info.out_dated:
    # build the object files if there were out dated
    apply(gcc, filter(is_out_dated, bin_info.deps))
    # build the binnary
    gcc(bin, option='')
else:
    # print message if up to date
    print('Up to date')
