# -*- using: utf-8 -*-


def twice(val):
    return val * 2


a = [1, 2, 3]
twice_list = list(map(twice, a))
print(twice_list)
