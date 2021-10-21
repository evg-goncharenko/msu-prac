from typing import List


def prod_non_zero_diag(X: List[List[int]]) -> int:
    """
    Compute product of nonzero elements from matrix diagonal, 
    return -1 if there is no such elements.

    Return type: int
    """    
    res = 1
    fl = -1

    for i in range(0, min(len(X), len(X[0]))):
        if X[i][i]:
            fl = 1
            res *= X[i][i]
    
    if fl == -1:
        return fl
    return res


def are_multisets_equal(x: List[int], y: List[int]) -> bool:
    """
    Return True if both 1-d arrays create equal multisets, False if not.

    Return type: bool
    """
    if len(x) != len(y):
        return False
    
    x.sort()
    y.sort()

    for i in range(len(x)):
        if x[i] != y[i]:
            return False

    return True


def max_after_zero(x: List[int]) -> int:
    """
    Find max element after zero in 1-d array, 
    return -1 if there is no such elements.

    Return type: int
    """
    max = -1
    if x != None:
        for i in range(1, len(x)):
            if (x[i-1] == 0) and (x[i] > max):
                max = x[i]
    return max


def convert_image(image: List[List[List[float]]], weights: List[float]) -> List[List[float]]:
    """
    Sum up image channels with weights.

    Return type: List[List[float]]
    """
    conv_image = [[0] * len(image[0]) for i in range(len(image))]

    for i in range(len(image)):
        for j in range(len(image[0])):
            for k in range (len(image[0][0])):
                conv_image[i][j] += image[i][j][k] * weights[k]
    
    return conv_image


def run_length_encoding(x: List[int]) -> (List[int], List[int]):
    """
    Make run-length encoding.

    Return type: (List[int], List[int])
    """
    compressed = []
    list_nums = []
    list_counts = []
    count = 1
    tmp = x[0]
    for i in range(1,len(x)):
        if x[i] == tmp:
            count = count + 1
        else:
            list_nums.append(tmp)
            list_counts.append(count)
            tmp = x[i]
            count = 1
    list_nums.append(tmp)
    list_counts.append(count)
    return (list_nums, list_counts)


def pairwise_distance(X: List[List[float]], Y: List[List[float]]) -> List[List[float]]:
    """
    Return pairwise object distance.

    Return type: List[List[float]]
    """
    matrix = [[0] * len(Y) for i in range(len(X))]

    for i in range(len(X)):
        for j in range(len(Y)):
            sum = 0
            for k in range(len(Y[j])):
                sum += (X[i][k] - Y[j][k]) * (X[i][k] - Y[j][k])
            matrix[i][j] = sum ** 0.5
    
    return matrix
