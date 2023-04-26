from solver import ListSolver
from math import sqrt

eps = 10 ** (-7)

n, m = [int(x) for x in input().split()]
c_list = list(map(int, input().split()))
p_list = list(map(int, input().split()))
w_list = []

for i in range(n):
    w_new_elem = [int(x) for x in input().split()]
    w_list.append(w_new_elem)

my_lambda = [1 for _ in range(m - 1)]

max_p = None
solver = ListSolver(n, c_list[-1])

w_last = []
for i in range(n):
    w_last.append(w_list[i][-1])

for step in range(30000):
    indexes = p_list.copy()
    for i in range(n):
        pair = list(zip(my_lambda, w_list[i][:-1]))
        cur_sum = sum(map(lambda x: x[0] * x[1], pair))
        indexes[i] -= cur_sum

    max_p, x_ind = solver.solve(w_last, indexes)

    for i in range(m - 1):
        max_p += my_lambda[i] * c_list[i]
    g_mas = c_list[:-1].copy()
    for i in range(m - 1):
        for j in x_ind:
            g_mas[i] -= w_list[j][i]

    g_norm = sqrt(sum([x * x for x in g_mas]))
    alpha = 1 / (1 + step)
    if g_norm < eps:
        break

    new_lambda = [my_lambda[i] - g_mas[i] * alpha / g_norm for i in range(m - 1)]
    my_lambda = [max(cur_l, 0) for cur_l in new_lambda]

print(max_p)
print(*my_lambda, sep='\n')
