import random as r
import math as m
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


def rand_normal(mu: float, sigma: float) -> float:
    """
    中身はrandom.normalvariateとほぼ同じ
    """

    z = m.sqrt(-2.0 * m.log2(r.random())) * m.sin(2.0 * 3.14 * r.random())
    return mu + sigma * z


def main():
    mu = 0
    sigma = 1
    values = np.array([rand_normal(mu, sigma) for _ in range(1000)])
    plt.hist(values, bins=30)
    plt.savefig("figure.png")


if __name__ == "__main__":
    main()
