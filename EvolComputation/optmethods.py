import numpy as np
from copy import deepcopy


def fa(func, M: int=30, D: int=5, tmax: int=1000,
       fend: float=1e-5, xmin: int=-5, xmax: int=5,
       sortver: bool=True):
    """
    @param M <int> : 粒子数
    @param D <int> : 解の次元数
    @param tmax <int> : 最大試行回数
    @param fend <float> : 終了条件
    @param xmin <int> : 初期値の最小値
    @param xmax <int> : 初期値の最大値
    @param sortver <bool> : ソートするかどうか
    ホタルアルゴリズムの実装
    """

    # 初期化処理
    # FAのパラメータ
    betamin = 0.2
    alpha0 = 0.5
    alpha = 0
    L = np.fabs(xmax - xmin) / 2
    gamma = 1 / np.sqrt(L)
    # 位置
    x = (xmin - xmax) * np.random.rand(M, D) + xmax
    xnew = np.zeros((M, D))
    # 解の評価値関数
    f = np.zeros(M)
    inverseF = np.zeros(M)
    # 目的関数の値
    xbest = np.zeros(D)
    fbest = float("inf")
    rij = float("inf")
    # その他のパラメータ
    k = 0
    # 可視化用
    xs = []
    fs = []

    # 関数の初期値の終了
    for i in range(M):
        f[i] = func(x[i])
        fbest = f[i] if f[i] < fbest else fbest

    # 実行
    for t in range(tmax):
        # aの更新
        alpha = alpha0 * ((1e-4 / 0.9) ** ((t + 1) / tmax))
        e = (np.random.rand() - 0.5) * L
        xnew = deepcopy(x)
        for i in range(M):
            f[i] = func(x[i])
            inverseF[i] = 1 / f[i]

        # ソートをかけて光強度が低い順にすすめていく
        if sortver:
            # sort
            k = 0
            tmp = list(zip(inverseF, xnew))
            inverseF, xnew = zip(*sorted(tmp, key=lambda a: a[0], reverse=True))
            inverseF = np.array(inverseF)
            xnew = np.array(xnew)
            for i in range(1, M)[::-1]:
                for j in range(0, i)[::-1]:
                    rij = np.linalg.norm(xnew[i] - x[j])
                    beta = (1 - betamin) * np.exp(-gamma * (rij ** 2)) + betamin
                    for l in range(D):
                        e = (np.random.rand() - 0.5) * L
                        xnew[i][l] += beta * (x[j][l] - xnew[i][l]) + alpha * e
            for l in range(D):
                e = (np.random.rand() - 0.5) * L
                xnew[0][l] += alpha * e
        # 順に光強度を参照していき，強い光に惹きつけられる
        else:
            # normal
            for i in range(M):
                cnt = 0
                for j in range(M):
                    if inverseF[i] < inverseF[j]:
                        cnt += 1
                        rij = np.linalg.norm(xnew[i] - x[j])
                        beta = (1 - betamin) * np.exp(-gamma * (rij ** 2)) + betamin
                        for l in range(D):
                            e = (np.random.rand() - 0.5) * L
                            xnew[i][l] += beta * (x[j][l] - xnew[i][l]) + alpha * e
                if cnt == 0:
                    k = i
                    for l in range(D):
                        e = (np.random.rand() - 0.5) * L
                        xnew[k][l] += alpha * e

        # 現在の最適解よりもいい解なら
        if f[k] < fbest:
            fbest = f[k]
            xbest = x[k]
        # 可視化用にパラメータを保持
        xs.append(xnew)
        fs.append(fbest)
        # 終了条件を満たしていたら
        if fbest < fend:
            break
        # ホタルの位置情報を更新
        x = deepcopy(xnew)

    return t + 1, fs


def de(func, M: int=30, D: int=5, cr: float=0.9, fw: float=0.5,
       tmax: int=1000, fend: float=1e-5, xmin: int=-5, xmax: int=5):
    """
    @param M <int> : 粒子数
    @param D <int> : 解の次元数
    @param cr <float> : DEのパラメータ
    @param fw <float> : DEのパラメータ
    @param tmax <int> : 最大試行回数
    @param xmin <int> : 初期値の最小値
    @param xmax <int> : 初期値の最大値
    遺伝的アルゴリズムのうちの差分進化の実装
    """

    # 初期化処理
    # 位置
    x = (xmin - xmax) * np.random.rand(M, D) + xmax
    xnew = np.zeros((M, D))
    # 更新に利用するパラメータ
    v = np.zeros(D)
    u = np.zeros(D)
    # 解の評価値関数
    f = np.zeros(M)
    # 目的関数の値
    ftmp = 0
    xbest = np.zeros(D)
    fbest = float("inf")
    xs = []
    fs = []
    m_numbers = range(M)
    # 関数の初期値
    for i in range(M):
        f[i] = func(x[i])
        fbest = f[i] if f[i] < fbest else fbest
    # 実行
    for t in range(tmax):
        for i in range(M):
            # iを除く3つの位置を取得
            randvec = np.random.choice(m_numbers, 3, replace=False)
            while i in randvec:
                randvec = np.random.choice(m_numbers, 3, replace=False)
            # 突然変異
            v = x[randvec[0]] + fw * (x[randvec[1]] - x[randvec[2]])
            # 1つだけはvを採用することを保証
            jr = np.random.randint(0, D)
            # 交叉
            for j in range(D):
                rj = np.random.rand()
                # u[j] = v[j] if rj < cr or jr == j else x[i][j]
                if rj < cr or jr == j:
                    u[j] = v[j]
                else:
                    u[j] = x[i][j]
            # uの評価関数の計算
            ftmp = func(u)
            # uを採用するかどうかの判定
            if ftmp < f[i]:
                f[i] = ftmp
                xnew[i] = u
                if ftmp < fbest:
                    fbest = ftmp
                    xbest = x[i]
            else:
                xnew[i] = x[i]
        # xの更新
        x = xnew
        # xとfの保持
        xs.append(deepcopy(x))
        fs.append(fbest)
        # 終了条件を満たしたとき
        if fbest < fend:
            break

    return t + 1, fs


def pso(func, M: int=30, D: int=5, c: float=1.494, w: float=0.729,
        tmax: int=1000, cr: float=1e-5, xmin: int=-5, xmax: int=5):
    """
    param <int> M : 粒子数
    param <int> D : 解の次元数
    param <float> c : PSOのパラメータ
    param <float> w : PSOのパラメータ
    param <int> Tmax : 最大試行回数
    param <float> Cr : 終了条件
    param <int> Xmin : 初期値の最小値
    param <int> Xmax : 初期値の最大値
    return <int> : 実行ステップ数
    return <float> : 方程式の値
    return <np.array> : 解のリスト
    粒子群最適化を行うメソッド
    """

    # 初期化処理
    # 位置
    x = (xmin - xmax) * np.random.rand(M, D) + xmax
    # 速度
    v = np.zeros((M, D))
    # 解の評価値関数
    f = np.zeros(M)
    # pbest
    fp = np.full(M, np.inf)
    xp = np.zeros((M, D))
    # gbest
    fg = float("inf")
    xg = np.zeros(D)
    # 目的関数の値
    fpbest = float("inf")
    fgbest = float("inf")
    fs = []
    xs = []

    # 実行
    for t in range(tmax):
        for i in range(M):
            f[i] = func(x[i])
            # pbest update
            if f[i] < fp[i]:
                fp[i] = f[i]
                for d in range(D):
                    xp[i][d] = x[i][d]
                # gbest update
                if fp[i] < fg:
                    xg = xp[i]
                    fg = func(xg)
        if fg < cr:
            break
        # pso update
        for i in range(M):
            for d in range(D):
                r1 = np.random.rand()
                r2 = np.random.rand()
                v[i][d] = w * v[i][d] + c * (r1 * (xp[i][d] - x[i][d]) + r2 * (xg[d] - x[i][d]))
                x[i][d] = x[i][d] + v[i][d]
        fs.append(fg)
        xs.append(deepcopy(x))

    return t + 1, fs


def sphere_func(x: list):
    return sum([val ** 2 for val in x])


def rastrigin_func(x: list):
    return sum([val ** 2 - 10 * np.cos(2 * np.pi * val) + 10 for val in x])
