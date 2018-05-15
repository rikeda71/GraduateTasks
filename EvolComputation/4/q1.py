from math import *


def standard_norm_dist(z: float):
    """
    標準正規分布の値を返す
    """

    return exp(-1 * z ** 2 / 2) / sqrt(2 * pi)


# 偏差値
divval = 53

# 平均
mean = 50

# 合格圏内の割合
zi = (divval - mean) / 10
print(standard_norm_dist(zi))
