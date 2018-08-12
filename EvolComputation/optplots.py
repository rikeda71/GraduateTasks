import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from optmethods import pso
from optmethods import fa
from optmethods import de
from optmethods import sphere_func
from optmethods import rastrigin_func


def graph_plot100(pso, de, fa, name: str="sample"):

    # 手法ごとに100回実行した中での最大実行回数の長さにリストを置き換える
    newpso = fill_maxlen_to_array(pso)
    newde = fill_maxlen_to_array(de)
    newfa = fill_maxlen_to_array(fa)

    maxlen = np.max([len(newpso[0]), len(newde[0]), len(newfa[0])])
    # 手法ごとに100回実行した中での平均の値を各イテレーションでの値にする
    lastpso = [np.mean([p[i] for p in newpso]) for i in range(len(newpso[0]))]
    lastde = [np.mean([d[i] for d in newde]) for i in range(len(newde[0]))]
    lastfa = [np.mean([f[i] for f in newfa]) for i in range(len(newfa[0]))]
    graph_plot(result=[lastpso, lastde, lastfa], name=name + "_100mean")


def graph_plot(result: list, labels: list=["pso", "de", "fa"], name: str="sample"):
    """
    @param t <int>   : ステップ回数
    @param fs <list> : 各ステップの関数の値群
    """

    fig = plt.figure()
    for fs, label in zip(result, labels):
        ts = np.array([i + 1 for i in range(len(fs))])
        plt.plot(ts, np.log(np.array(fs)), label=label)
    plt.title(name)
    plt.xlabel("t(iteration)")
    plt.ylabel("fbest")
    plt.legend(bbox_to_anchor=(1, 1), borderaxespad=0)
    plt.savefig(name + ".png")


def fill_in_value(array: list, t: int=1000):
    last = array[-1]
    array += [last] * (t - len(array))
    return array


def fill_maxlen_to_array(array: list) -> list:
    lenlist = [len(a) for a in array]
    maxlen = np.max(lenlist)
    return [fill_in_value(a, maxlen) for a in array]


print("pso")
pso_sphere2 = [pso(func=sphere_func, D=2)[-1] for _ in range(100)]
pso_rastrigin2 = [pso(func=rastrigin_func, D=2)[-1] for _ in range(100)]
pso_sphere20 = [pso(func=sphere_func, D=20)[-1] for _ in range(100)]
pso_rastrigin20 = [pso(func=rastrigin_func, D=20)[-1] for _ in range(100)]

print("de")
de_sphere2 = [de(func=sphere_func, D=2)[-1] for _ in range(100)]
de_rastrigin2 = [de(func=rastrigin_func, D=2)[-1] for _ in range(100)]
de_sphere20 = [de(func=sphere_func, D=20)[-1] for _ in range(100)]
de_rastrigin20 = [de(func=rastrigin_func, D=20)[-1] for _ in range(100)]

print("fa")
fa_sphere2 = [fa(func=sphere_func, D=2, sortver=False)[-1] for _ in range(100)]
fa_rastrigin2 = [fa(func=rastrigin_func, D=2, sortver=False)[-1] for _ in range(100)]
fa_sphere20 = [fa(func=sphere_func, D=20, sortver=False)[-1] for _ in range(100)]
fa_rastrigin20 = [fa(func=rastrigin_func, D=20, sortver=False)[-1] for _ in range(100)]

# sphere2
li = [pso_sphere2[0], de_sphere2[0], fa_sphere2[0]]
graph_plot(result=li, name="sphere2")
graph_plot100(pso_sphere2, de_sphere2, fa_sphere2, "sphere2")
# rastrigin2
li = [pso_rastrigin2[0], de_rastrigin2[0], fa_rastrigin2[0]]
graph_plot(result=li, name="rastrigin2")
graph_plot100(pso_rastrigin2, de_rastrigin2, fa_rastrigin2, "rastrigin2")
# sphere20
li = [pso_sphere20[0], de_sphere20[0], fa_sphere20[0]]
graph_plot(result=li, name="sphere20")
graph_plot100(pso_sphere20, de_sphere20, fa_sphere20, "sphere20")
# rastrigin20
li = [pso_rastrigin20[0], de_rastrigin20[0], fa_rastrigin20[0]]
graph_plot(result=li, name="rastrigin20")
graph_plot100(pso_rastrigin20, de_rastrigin20, fa_rastrigin20, "rastrigin20")
