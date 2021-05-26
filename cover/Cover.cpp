from collections import namedtuple as nt
from scipy.optimize import linprog as lp
import numpy as np


row = nt('row', 'ind weight covered_c')


def find_value(rows, not_colrs, cover):
    
    covered = set()
    value = 0
    
    for r in rows:
        if not cover[r.ind]:
            continue
            
        for c in r.covered_c:
            covered.add(c)
            
        value += r.weight
        
    return (len(covered) == not_colrs, value)


def sol(rows, not_colrs):
    not_colovreed_rows = len(rows)
    weights = np.array([r.weight for r in rows])
    incidence = np.zeros((not_colovreed_rows, not_colrs), 'int')
    
    for r in rows:
        for c in r.covered_c:
            incidence[r.ind][c] = 1
            
    solution = lp(weights, A_ub=-incidence.T, b_ub=-
                       np.ones((not_colrs, 1)), bounds=(0, None))
    n_iters = 100_000
    best_cover = None
    best_value = None
    
    for _ in range(n_iters):
        cover = np.random.rand(not_colovreed_rows) > solution.x
        is_cover, value = find_value(rows, not_colrs, cover)
        
        if is_cover and (best_value is None or best_value > value):
            best_value = value
            best_cover = cover
            
    if best_value is None:
        print('Solution not found!')
        
    else:
        answer = np.arange(1, not_colovreed_rows + 1)[best_cover]
        print(' '.join(map(str, answer)))


def solve():
    not_colrs, not_colovreed_rows = map(int, input().split())
    rows = []
    
    for i in range(not_colovreed_rows):
        weight, *covered_c = input().split()
        rows.append(row(i, int(weight), np.array(covered_c, 'int')))
        
    sol(rows, not_colrs)


if __name__ == '__main__': #idk why it works
    solve()
