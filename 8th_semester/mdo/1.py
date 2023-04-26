answer = []

with open("input.txt", "r") as file:
    contents = file.read().splitlines()

n, c = [int(x) for x in contents[0].split()]
result = dict()
result[0] = [(-1, -1, 0, 0)]

for i in range(1, n+1):
    w, p = [int(x) for x in contents[i].split()]
    
    tuple_list = []
    tuple_list2 = []

    last = result[i - 1]
    max_weight = 0

    for j in range(len(last)):
        tupl = (0, j+1, last[j][2], last[j][3])
        tuple_list.append(tupl)

        if tupl[2] + w <= c:
            elem = (1, j+1, tupl[2] + w, tupl[3] + p)

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

max_val = result[n][-1][3]
p = result[n][-1][1]

if result[n][-1][0] == 1:
    answer.append(n-1)

for i in range(n - 1, 0, -1):
    if result[i][p - 1][0] == 1:
        answer.append(i - 1)

    p = result[i][p - 1][1]

print(*[max_val, len(answer)], sep=' ')
print(*answer, sep='\n')
