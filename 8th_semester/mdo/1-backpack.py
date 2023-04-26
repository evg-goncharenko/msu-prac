n, C = [int(x) for x in input().split()]
w_list = []
p_list = []
for i in range(n):
    w_new_elem, p_new_elem = [int(x) for x in input().split()]
    w_list.append(w_new_elem)
    p_list.append(p_new_elem)

my_dict = {-1: [[-1, -1, 0, 0]]}
for step_number in range(n):
    list_0 = []
    list_1 = []
    for elem_index, elem in enumerate(my_dict[step_number - 1]):
        w_value = elem[2]
        list_0.append([0, elem_index, w_value, elem[3]])
        if w_value + w_list[step_number] <= C:
            list_1.append(
                [1, elem_index, w_value + w_list[step_number], elem[3] + p_list[step_number]])

    index_0 = 0
    index_1 = 0
    result = []
    while index_0 < len(list_0) and index_1 < len(list_1):
        if list_0[index_0][2] > list_1[index_1][2]:
            result.append(list_1[index_1])
            index_1 += 1
        elif list_0[index_0][2] < list_1[index_1][2]:
            result.append(list_0[index_0])
            index_0 += 1
        else:
            if list_0[index_0][3] < list_1[index_1][3]:
                index_0 += 1
            else:
                index_1 += 1
        if index_0 == len(list_0):
            result.extend(list_1[index_1:])
        if index_1 == len(list_1):
            result.extend(list_0[index_0:])

    my_dict[step_number] = result

max_p_elem = max(my_dict[n - 1], key=lambda x: x[3])

solution_list = []
elem = max_p_elem
for i in range(n - 1, -1, -1):
    if elem[0] != 0:
        solution_list.append(i)
    prev = elem[1]
    elem = my_dict[i - 1][prev]

print(max_p_elem[3], len(solution_list))
for i in solution_list:
    print(i)
