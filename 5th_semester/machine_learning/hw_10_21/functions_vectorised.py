import numpy as np


def prod_non_zero_diag(X: np.ndarray) -> int:
    """
    Compute product of nonzero elements from matrix diagonal, 
    return -1 if there is no such elements.
    
    Return type: int / np.integer / np.int32 / np.int64
    """
    y = np.diag(X)[np.diag(X) != 0]
    res = 1
    fl = -1

    for i in y:
        fl = 1
        res *= i
    
    if fl == -1:
        return fl
    return res


def are_multisets_equal(x: np.ndarray, y: np.ndarray) -> bool:
    """
    Return True if both 1-d arrays create equal multisets, False if not.
    
    Return type: bool / np.bool_
    """
    return np.array_equiv(np.sort(x), np.sort(y))


def max_after_zero(x: np.ndarray) -> int:
    """
    Find max element after zero in 1-d array, 
    return -1 if there is no such elements.

    Return type: int / np.integer / np.int32 / np.int64
    """
    x = np.array(x)
    y = x[1:]
    y = y[(x == 0)[:-1]] 
    if y.size == 0:
        return -1
    return np.amax(y)


def convert_image(image: np.ndarray, weights: np.ndarray) -> np.ndarray:
    """
    Sum up image channels with weights.

    Return type: np.ndarray
    """
    return np.sum(image * weights[None, None, :], axis = -1)


def run_length_encoding(x: np.ndarray) -> (np.ndarray, np.ndarray):
    """
    Make run-length encoding.

    Return type: (np.ndarray, np.ndarray)
    """
    y = x[1:] != x[:-1] # finding not equal numbers
    i = np.append(np.where(y), len(x) - 1)
    
    list_nums = x[i]
    list_counts = np.diff(np.append(-1, i))

    return(list_nums, list_counts)


def pairwise_distance(X: np.ndarray, Y: np.ndarray) -> np.ndarray:
    """
    Return pairwise object distance.

    Return type: np.ndarray
    """
    P = np.add.outer(np.sum(X**2, axis = 1), np.sum(Y**2, axis = 1))
    N = np.dot(X, Y.T)
    return np.sqrt(P - 2 * N)
