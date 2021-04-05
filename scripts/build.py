import argparse
from buidler import Builder

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-main', default='src/main.c', type=str)
    parser.add_argument('-out', default='main', type=str)
    parser.add_argument('-src', default='src', type=str)
    parser.add_argument('-obj', default='obj', type=str)
    parser.add_argument('-cc', default='gcc', type=str)
    parser.add_argument('-cflag', default='')
    args = parser.parse_args().__dict__
    print(args)
    Builder(**args)
