from tensorboardX import SummaryWriter
import fire
from pathlib import Path


def plot(path=""):
    path = Path(path).absolute()
    print(f'Reading {path}')
    if not path.exists():
        print(f'{path} does not exist')
        return
    paths = [p for p in path.iterdir() if not p.is_file()]
    paths = [(p, float(p.name)) for p in paths]
    paths = sorted(paths, key=lambda tup: tup[1])

    out_path = Path('loss_plots').joinpath(str(path)[1:])

    writer = SummaryWriter(str(out_path))
    for i, p in enumerate(paths):
        writer.add_scalar(str(path)[1:], p[1], i)


def plot_all(path=""):
    path = Path(path).absolute()
    if not path.exists():
        print(f'{path} does not exist')
        return
    for p in path.iterdir():
        not p.is_file() and plot(p)


if __name__ == '__main__':
    fire.Fire(plot_all)
