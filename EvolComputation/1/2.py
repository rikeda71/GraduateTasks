import numpy as np
import sys
sys.path.append("../")


def func1(x1: float, x2: float):
    return x1 ** 2 + x2 ** 2 - 1


def diff_x1_func1(x1: float, x2: float):
    return 2 * x1


def diff_x2_func1(x1: float, x2: float):
    return 2 * x2


def func2(x1: float, x2: float):
    return x1 ** 3 - x2


def diff_x1_func2(x1: float, x2: float):
    return 3 * (x1 ** 2)


def diff_x2_func2(x1: float, x2: float):
    return -1


def main():
    e = 1e-6
    i = 0
    x = np.array([1, 0.5])
    while True:
        x1 = x[0]
        x2 = x[1]
        # 更新式
        j = np.array([[diff_x1_func1(x1, x2), diff_x2_func1(x1, x2)],
                     [diff_x1_func2(x1, x2), diff_x2_func2(x1, x2)]])
        print(np.linalg.inv(j))
        x -= np.dot(np.linalg.inv(j), np.array([func1(x1, x2), func2(x1, x2)]))
        # 状態の出力
        print("x1_" + str(i) + ":" + str(x[0]))
        print("x2_" + str(i) + ":" + str(x[1]))
        print("f1(x_" + str(i) + "):" + str(func1(x[0], x[1])))
        print("f2(x_" + str(i) + "):" + str(func2(x[0], x[1])))
        print("-------------------")
        # 終了条件
        if abs(func1(x[0], x[1])) < e and abs(func2(x[0], x[1])) < e:
            break
        i += 1
    # 最終結果の出力
    print(func1(x[0], x[1]))
    print(func2(x[0], x[1]))


if __name__ == "__main__":
    main()
