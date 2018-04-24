import random


def function(x: float):
    return (x ** 3) + 2 * (x ** 2) - 5 * x + 6


def markovchain_opt(f) -> float:
    """
    markov chain alogorithmによる
    引数の関数の値が0になる点の最適化
    """

    e = 0.001
    x_t1 = random.random()
    t = 0
    # 最適化
    while abs(f(x_t1)) > e:
        t += 1
        x_t = x_t1
        y = x_t + random.normalvariate(0, 1)
        pt = random.random()
        x_t1 = y if random.random() < pt else x_t
        if t % 10000 == 0:
            print(str(t) + ":" + str(x_t1))
    print(t)
    return x_t1


def main():
    random.seed()
    x = markovchain_opt(function)
    print(x)


if __name__ == "__main__":
    main()
