import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation


def graph_plot(t: int, fs: list, name: str="sample"):
    """
    @param t <int>   : ステップ回数
    @param fs <list> : 各ステップの関数の値群
    """

    fig = plt.figure()
    ts = np.array([i + 1 for i in range(t - 1)])
    plt.plot(ts, np.array(fs))
    plt.savefig(name + ".png")


def dots_anim_plot(xs: list, name: str="sample"):
    """
    @param xs <list> : 各ステップにおける値群
    """

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    imgs = []
    for i, dots in enumerate(xs):
        dots_x = [dot[0] for dot in dots]
        dots_y = [dot[1] for dot in dots]
        img = ax.scatter(dots_x, dots_y, c="red")
        title = ax.text(0.5, 1.05, name + " : t =  {}".format(str(i + 1)),
                        size=plt.rcParams["axes.titlesize"],
                        ha="center", transform=ax.transAxes, )
        imgs.append([img, title])
    ax.set_xlim(-5, 5)
    ax.set_ylim(-5, 5)
    ax.grid()
    ax.set_xlabel('x1')
    ax.set_ylabel('x2')
    ax.scatter(dots_x[0], dots_x[1], c="blue")
    ani = animation.ArtistAnimation(fig, imgs, interval=100)
    ani.save(name + ".gif", writer="imagemagick")
