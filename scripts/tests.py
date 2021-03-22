import argparse
from pathlib import Path
from buidler import Builder
from os import scandir


def tests_dir(path):
    l = []
    for d in path.iterdir():
        if not d.is_dir():
            continue
        if d.name == 'tests':
            l.append(d)
        else:
            l += tests_dir(d)
    return l


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-outs', default='tests/', type=str)
    parser.add_argument('-src', default='src', type=str)
    parser.add_argument('-obj', default='obj', type=str)
    parser.add_argument('-cc', default='gcc', type=str)
    parser.add_argument('-cflag', default='')

    args = parser.parse_args()
    outs = Path(args.outs)
    src = Path(args.src)
    tests = tests_dir(src)

    for test in tests:
        module = test.parent.name
        test_main = test / f'{module}_test.c'
        print(test)
        print('=================')
        if test_main.exists():
            Builder(
                test_main,
                outs/module,
                args.src,
                args.obj,
                args.cc,
                args.cflag
            )
        else:
            print("No entry file:", test_main)
        print()
