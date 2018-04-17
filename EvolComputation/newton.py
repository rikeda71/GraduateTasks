def newton_update(x: float, f, df) -> float:
    return x - f(x) / df(x)
