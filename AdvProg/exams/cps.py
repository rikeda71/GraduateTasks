# -*- using: utf-8 -*-


def power(n, m):
    if m == 0:
        return 1
    else:
        return power(n, m - 1) * n


def aux(n, c):
    return lambda r: c(n * r)


def powerCPS(n, m, c):
    if m == 0:
        return 1
    while(m != 0):
        c = aux(n, c)
        m -= 1
    return c(1)


print(power(5, 3))
print(powerCPS(5, 3, lambda x: x))
