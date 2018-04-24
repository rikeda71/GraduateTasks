import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import sys
sys.path.append("../")
from newton import newton_update


def function(x: float):
    return (x ** 3) + 2 * (x ** 2) - 5 * x + 6


def diff_function(x: float):
    return 3 * (x ** 2) + 4 * x - 5


def main():
    e = 0.001
    # -3や1を入力
    print("初期値を入力")
    x = int(input())
    i = 0
    while abs(function(x)) > e:
        print("x" + str(i) + ":" + str(x))
        print("f(x" + str(i) + "):" + str(function(x)))
        x = newton_update(x, function, diff_function)
        i += 1
    print("x" + str(i) + ":" + str(x))
    print("f(x" + str(i) + "):" + str(function(x)))
    x = np.arange(-5, 3, 0.01)
    y = np.array([function(v) for v in x])
    plt.plot(x, y)
    plt.title("x^3 + 2x^2 - 5x +6(-5 <= x <= 3)")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.savefig("figure.png")


if __name__ == "__main__":
    main()
