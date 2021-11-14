import numpy as np
from collections import defaultdict

def kfold_split(num_objects, num_folds):
    """Split [0, 1, ..., num_objects - 1] into equal num_folds folds (last fold can be longer) and returns num_folds train-val 
       pairs of indexes.

    Parameters:
    num_objects (int): number of objects in train set
    num_folds (int): number of folds for cross-validation split

    Returns:
    list((tuple(np.array, np.array))): list of length num_folds, where i-th element of list contains tuple of 2 numpy arrays,
                                       the 1st numpy array contains all indexes without i-th fold while the 2nd one contains
                                       i-th fold
    """
    mass = []
    group_size = num_objects // num_folds
    pos = [i for i in range(num_objects)]
    for i in range(num_folds):
        first = pos[:(i * group_size)]
        if i == (num_folds - 1):
            second = pos[i * group_size:]
        else:
            first += (pos[(i + 1) * group_size:])
            second = pos[i * group_size:(i + 1) * group_size]

        mass.append((np.array(first), np.array(second)))
    
    return mass

def knn_cv_score(X, y, parameters, score_function, folds, knn_class):
    """Takes train data, counts cross-validation score over grid of parameters (all possible parameters combinations) 

    Parameters:
    X (2d np.array): train set
    y (1d np.array): train labels
    parameters (dict): dict with keys from {n_neighbors, metrics, weights, normalizers}, values of type list,
                       parameters['normalizers'] contains tuples (normalizer, normalizer_name), see parameters
                       example in your jupyter notebook
    score_function (callable): function with input (y_true, y_predict) which outputs score metric
    folds (list): output of kfold_split
    knn_class (obj): class of knn model to fit

    Returns:
    dict: key - tuple of (normalizer_name, n_neighbors, metric, weight), value - mean score over all folds
    """
    dict = {}
    total_size = len(folds)

    for neighbour in parameters['n_neighbors']:
        for weight in parameters['weights']:
            for metric in parameters['metrics']:
                for normalizer in parameters['normalizers']:
                    summ = 0

                    for fold in folds:
                        x_train = X[fold[0]]
                        y_train = y[fold[0]]
                        x_test = X[fold[1]]
                        y_test = y[fold[1]]

                        if normalizer[0] != None:
                            normalizer[0].fit(x_train)
                            x_train = normalizer[0].transform(x_train)
                            x_test = normalizer[0].transform(x_test)

                        model = knn_class(n_neighbors = neighbour, metric = metric, weights = weight)
                        model.fit(x_train, y_train)
                        y_predict = model.predict(x_test)
                    
                        summ += score_function(y_test, y_predict)
                    
                    dict[(normalizer[1], neighbour, metric, weight)] = summ / total_size
    
    return dict
