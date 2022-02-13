import numpy as np

from numpy.testing import assert_equal, assert_allclose
from sklearn import neighbors
from sklearn.metrics import r2_score
from cross_val import kfold_split, knn_cv_score


def test_split_0():
    with open('cross_val.py', 'r') as file:
        lines = ' '.join(file.readlines())
        assert 'import numpy' in lines
        assert 'import defaultdict' in lines
        assert lines.count('import') == 2
        assert 'sklearn' not in lines

def test_split_1():
    X_1 = kfold_split(2, 2)
    answer = [(np.array([1]), np.array([0])), (np.array([0]), np.array([1]))]

    assert type(X_1) == list
    assert_equal(X_1, answer)

def test_split_2():
    X_1 = kfold_split(5, 3)
    answer = [(np.array([1, 2, 3, 4]), np.array([0])),
              (np.array([0, 2, 3, 4]), np.array([1])),
              (np.array([0, 1]), np.array([2, 3, 4]))]

    assert type(X_1) == list
    assert_equal(X_1, answer)

def test_cv_3():
    X_train = np.array([[2, 1, -1], [1, 1, 1], [0.9, -0.25, 7], [1, 2, -3], [0, 0, 0], [2, -1, 0.5]])
    y_train = np.sum(X_train, axis=1)

    X_train = np.array([[2, 1, -1], [1, 1, 1], [0.9, -0.25, 7], [1, 2, -3], [0, 0, 0], [2, -1, 0.5]])
    y_train = np.sum(X_train, axis=1)
    parameters = {
        'n_neighbors': [1, 2, 4],
        'metrics': ['euclidean', 'cosine'],
        'weights': ['uniform', 'distance'],
        'normalizers': [(None, 'None')]
    }
    folds = kfold_split(6, 3)
    out = knn_cv_score(X_train, y_train, parameters, r2_score, folds, neighbors.KNeighborsRegressor)

    answer = {
        ('None', 1, 'euclidean', 'uniform'): -11.29188203967135,
        ('None', 1, 'euclidean', 'distance'): -11.29188203967135,
        ('None', 1, 'cosine', 'uniform'): -17.63280796559728,
        ('None', 1, 'cosine', 'distance'): -17.632807965597276,
        ('None', 2, 'euclidean', 'uniform'): -7.5333863756105215,
        ('None', 2, 'euclidean', 'distance'): -7.997305328982919,
        ('None', 2, 'cosine', 'uniform'): -4.246942433109774,
        ('None', 2, 'cosine', 'distance'): -6.7448165099645365,
        ('None', 4, 'euclidean', 'uniform'): -3.6194607932134364,
        ('None', 4, 'euclidean', 'distance'): -4.211377791660151,
        ('None', 4, 'cosine', 'uniform'): -3.6194607932134364,
        ('None', 4, 'cosine', 'distance'): -4.335691384752842
     }

    assert type(out) == dict
    assert len(out) == len(answer)
    for key in answer:
        assert_allclose(answer[key], out[key])
