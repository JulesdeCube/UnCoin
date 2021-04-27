from pathlib import Path
from datetime import datetime
import os
import sys
import re


def apply(func, *iterables):
    """ Apply function by getting one element of each elements of iterables. """
    for args in zip(*iterables):
        func(*args)


def _p(path):
    return Path(path).resolve()


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

    @property
    def out_dated(self):
        """ If the file out dated """
        return self.date < self.deps_date


class Builder:

    regex = re.compile('^#include "([^"]*)"$')

    def __init__(self, main, out, src, obj, cc, cflag):
        self.main = _p(main)
        self.out = _p(out)
        self.src = _p(src)
        self.obj = _p(obj)
        self.cc = cc
        self.cflag = f'-I {src} {cflag}'

        self.infos = dict()

        # build the depencies graph
        self.proccess(self.main)
        # get info the the binnary
        bin_info = self.infos[self.out]
        # upate to get it's dependecies date
        self.update(self.out)

        # if the binnary is up to date do noting
        if bin_info.out_dated:
            # build the object files if there were out dated
            apply(self.gcc, filter(self.is_out_dated, bin_info.deps))
            # build the binnary
            self.gcc(self.out, option='')
        else:
            # print message if up to date
            print('Up to date')

    def deps_date(self, path):
        """ The depencies max date of the file. """
        return self.infos[path].deps_date

    def is_out_dated(self, path):
        """ If the file is out dated """
        return self.infos[path].out_dated

    def add(self, path):
        """ Add the file into the infos and return if we added or not. """
        # If file is already init return false
        if path in self.infos:
            return False
        # If it's not inside init it and return true
        else:
            self.infos[path] = Info(path)
            return True

    def link(self, src, dep):
        """ Mark "dep" as a Dependence of the "src". """
        self.add(src)
        self.infos[src].deps.add(dep)

    @staticmethod
    def get_deps(file):
        """ Get the list of the depedency of the file. """
        # deps list
        deps = []

        # get each match for each line
        for match in map(Builder.regex.match, file):
            # if there is a match
            if match:
                # add it to the dependecy
                deps.append(match.group(1))

        # retur the deps list (string)
        return deps

    def update(self, path):
        """ Update the deps_date atribute. """
        # the max bwtween it's self and it's dependy dependities dates
        info = self.infos[path]
        info.deps_date = max(
            [info.date] +
            list(map(self.deps_date, info.deps))
        )

    def expend(self, src, dst):
        # get the prefix
        start = dst[0]

        # if it is a relative path
        if start == '.':
            # calcule path relative to the source file
            path = src.parent / dst
        # if it's a absolute path
        elif start == '/':
            # use the root source dir as reference
            path = self.src / dst
        # if no indication
        else:
            # test each of the possible origins
            path = src.parent / dst  # relative
            if not path.exists():
                path = self.src / dst  # absolute

        # if the path didn't exist return an error
        if not path.exists():
            raise ValueError(f'ERROR: can\'t find "{dst}" from "{src}"')

        # normalize path to not have the dots paths
        path = path.resolve()

        # return the expended path
        return path

    @staticmethod
    def is_source_file(path):
        """ return if the file is a source files """
        return path.suffix in ['.c', '.h']

    def get_obj(self, path):
        """ Get the object file path base on the source file. """
        # get the path relative to the src path
        src = path.relative_to(self.src)
        # create the object path by using the object dirrectory and replace the
        # suffix
        obj = self.obj / src.with_suffix('.o')
        # normalize the path
        obj.resolve()
        # return it
        return obj

    def parse_source(self, path):
        """ Add all the dependecy by getting the depency of the source file """
        with open(path, 'r', encoding='utf8') as file:
            for dep in self.get_deps(file):
                dep = self.expend(path, dep)
                self.proccess(dep)
                self.link(path, dep)

    def proccess_module(self, path):
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
                for dep in filter(self.is_source_file, folder.iterdir()):
                    self.proccess(dep)
                    self.link(path, dep)

        self.update(path)

    def proccess(self, path):
        """
        Get reclusively all the dependency of the path and compute it's dependency
        date.
        """
        # proccess one time each path
        # don't proccess not existing path
        if self.add(path) and path.exists():
            # get and proccess each depedency of the file
            self.parse_source(path)
            # update the dependency dates
            self.update(path)

            # back propagate the header files

            # if it is a header get the corresponding source file
            if path.suffix == '.h':
                # proccess basics include
                self.proccess(path.with_suffix('.c'))
                # proccess module
                self.proccess_module(path)

            # if it is a source file add the object and linked it to the binnary
            if path.suffix == '.c':
                # get object path
                obj = self.get_obj(path)
                # link the source to the object
                self.link(obj, path)
                # link the object to the binnaty
                self.link(self.out, obj)
                # update the object infos
                self.update(obj)

    @staticmethod
    def run_command(command):
        """ Print and run a command. """
        # show the command
        print(' '.join(command))
        # execute the command
        result = os.system(' '.join(command))
        # if there is an error return it
        if result != 0:
            exit(1)

    def gcc(self, path, option='-c'):
        """ Compile the source file base on the option. """
        # create output dirrectory if not exist
        path.parent.mkdir(parents=True, exist_ok=True)

        # the current dir
        cwd = os.getcwd()

        def get_path(path):
            """ Shotten the path and covert it to string. """
            return str(path.relative_to(cwd))

        # the the gcc command with the corresponding option
        self.run_command([
            self.cc,
            option, ' '.join(map(get_path, self.infos[path].deps)),
            '-o', get_path(path),
            self.cflag
        ])
