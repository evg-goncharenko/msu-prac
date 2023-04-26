import math


class ListSolver:

    def __init__(self, n, c):
        self.n = n
        self.c = c

    def solve(self, weights, price):

            answer = []


            n, c = self.n, self.c

            result = dict()

            result[0] = [(-1, -1, 0, 0)]

            for i in range(1, n+1):

                w, p = weights[i-1], price[i-1]

                tuple_list = []

                tuple_list2 = []

                last = result[i - 1]

                max_weight = 0

                for j in range(len(last)):

                    tupl = (0, j + 1, last[j][2], last[j][3])

                    tuple_list.append(tupl)

                    if tupl[2] + w <= c:

                        elem = (1, j + 1, tupl[2] + w, tupl[3] + p)

                        if elem[2] > max_weight:
                            max_weight = elem[2]

                        tuple_list2.append(elem)

                size_1 = len(tuple_list)
                size_2 = len(tuple_list2)

                res = []
                o, j = 0, 0

                while o < size_1 and j < size_2:
                    if tuple_list2[j][2] > max_weight:
                        j += 1
                        continue
                    if tuple_list[o][2] <= tuple_list2[j][2] and tuple_list[o][3] >= tuple_list2[j][3]:
                        j += 1
                    elif tuple_list2[j][2] <= tuple_list[o][2] and tuple_list2[j][3] >= tuple_list[o][3]:
                        o += 1
                    elif tuple_list[o][3] < tuple_list2[j][3]:
                        res.append(tuple_list[o])
                        o += 1
                    else:
                        res.append(tuple_list2[j])
                        j += 1

                res = res + tuple_list[o:] + tuple_list2[j:]

                result[i] = res

            ans = [0] *(len(result)-1)

            maximum = result[n][-1][3]
            pr = result[n][-1][1]

            if result[n][-1][0] == 1:
                ans[n-1] = 1
                answer.append(n - 1)

            for i in range(n - 1, 0, -1):

                if result[i][pr - 1][0] == 1:
                    ans[i-1] = 1
                    answer.append(i - 1)

                pr = result[i][pr - 1][1]

            return maximum, ans

with open("input.txt", "r") as file:
    contents = file.read().splitlines()

n, m = [int(x) for x in contents[0].split()]
c = [int(x) for x in contents[1].split()]
p = [int(x) for x in contents[2].split()]

w = []
for s in contents[3:]:
    temp = [int(x) for x in s.split()]
    w.append(temp)

lambda_ = [1] * (m - 1)

q = [x[-1] for x in w]

d_old = 0
d = 0

for t in range(1, 9999):

    coef = []
    sub_grad = []

    for j in range(n):
        res1 = 0

        for l in range(m - 1):
            res1 += lambda_[l] * w[j][l]

        coef.append(p[j] - res1)

    solv = ListSolver(n, c[-1])
    max_p, answer = solv.solve(q, coef)

    for j in range(m - 1):
        temp = 0

        for l in range(len(answer)):

            temp += w[l][j] * answer[l]

        sub_grad.append(c[j] - temp)

    c_ = sum([c[j] * lambda_[j] for j in range(m - 1)])

    d = c_ + max_p

    if abs(d_old - d) < 0.001:
        break

    norma = 0

    for j in range(m - 1):
        norma += sub_grad[j] * sub_grad[j]

    norma = math.sqrt(norma)

    for j in range(m - 1):

        if norma == 0:
            norma += 0.001

        lambda_[j] -= (1 / norma / t) * sub_grad[j]
        lambda_[j] = max(lambda_[j], 0)

    d_old = d

print(d)
print(*lambda_, sep='\n')
