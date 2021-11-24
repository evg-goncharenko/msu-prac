import numpy as np


class Preprocesser:
    
    def __init__(self):
        pass
    
    def fit(self, X, Y=None):
        pass
    
    def transform(self, X):
        pass
    
    def fit_transform(self, X, Y=None):
        pass
    
    
class MyOneHotEncoder(Preprocesser):
    
    def __init__(self, dtype=np.float64):
        super(Preprocesser).__init__()
        self.dtype = dtype
        self.array = []
        
    def fit(self, X, Y=None):
        """
        param X: training objects, pandas-dataframe, shape [n_objects, n_features]
        param Y: unused
        """
        for elem in X:
            self.array.append(np.unique(X[elem]))
    
    def transform(self, X):
        """
        param X: objects to transform, pandas-dataframe, shape [n_objects, n_features]
        returns: transformed objects, numpy-array, shape [n_objects, |f1| + |f2| + ...]
        """
        l = []
        for i in self.array:
           l.append(len(i)) 
        result = np.zeros((len(X), sum(l)))
        pos = 0
        summ = 0 
        index = 0 
        index2 = 0
        for i in X:
            for ind in X.index:
                a = X.loc[ind]
                result[pos % len(result)][list(self.array[index]).index(a[i]) + summ] = 1
                pos += 1
            summ += l[index2]
            index2 += 1
            index += 1
        return result
    
    def fit_transform(self, X, Y=None):
        self.fit(X)
        return self.transform(X)
    
    def get_params(self, deep=True):
        return {"dtype": self.dtype}
    
    
class SimpleCounterEncoder:
    
    def __init__(self, dtype=np.float64):
        self.dtype=dtype
        self.array = []
        
    def fit(self, X, Y):
        """
        param X: training objects, pandas-dataframe, shape [n_objects, n_features]
        param Y: target for training objects, pandas-series, shape [n_objects,]
        """
        for i in X:
            _ , new_list = np.unique(X[i], return_counts = True)
            index = 0
            self.array.append({})
            for j in np.unique(X[i]):
                val = new_list[index]
                k = Y.loc[X[j == X[i]].index]
                self.array[-1][j] = (sum(k) / val, val / len(X.index))
                index += 1

            new_list = []
            
    def transform(self, X, a=1e-5, b=1e-5):
        """
        param X: objects to transform, pandas-dataframe, shape [n_objects, n_features]
        param a: constant for counters, float
        param b: constant for counters, float
        returns: transformed objects, numpy-array, shape [n_objects, 3]
        """
        init_pos = len(X.columns) * 3
        result = np.zeros((len(X), init_pos))        
        ind = 0
        ind2 = 0
        pos = 0
        new_arr = self.array[pos]
        for i in X:
            for j in X.index:
                new_arr = self.array[pos]
                new_arr2 = result[ind2]
                tmp_val = X.loc[j][i]
                new_arr2[ind + 2] = (new_arr[tmp_val][0] + a) / (new_arr[tmp_val][1] + b)
                result[ind2] = new_arr2
                new_arr2[ind + 1] = new_arr[tmp_val][1]
                result[ind2] = new_arr2
                new_arr2[ind + 0] = new_arr[tmp_val][0]
                result[ind2] = new_arr2
                ind2 += 1
            ind += 3
            pos += 1
            ind2 = 0
        return result
    
    def fit_transform(self, X, Y, a=1e-5, b=1e-5):
        self.fit(X, Y)
        return self.transform(X, a, b)
    
    def get_params(self, deep=True):
        return {"dtype": self.dtype}

    
def group_k_fold(size, n_splits=3, seed=1):
    idx = np.arange(size)
    np.random.seed(seed)
    idx = np.random.permutation(idx)
    n_ = size // n_splits
    for i in range(n_splits - 1):
        yield idx[i * n_ : (i + 1) * n_], np.hstack((idx[:i * n_], idx[(i + 1) * n_:]))
    yield idx[(n_splits - 1) * n_ :], idx[:(n_splits - 1) * n_]

    
class FoldCounters:
    
    def __init__(self, n_folds=3, dtype=np.float64):
        self.dtype = dtype
        self.n_folds = n_folds
        self.array = []

    def fit(self, X, Y, seed=1):
        """
        param X: training objects, pandas-dataframe, shape [n_objects, n_features]
        param Y: target for training objects, pandas-series, shape [n_objects,]
        param seed: random seed, int
        """
        self.array = np.zeros((len(X), len(X.columns) * 3))
        array = np.array(X)
        index = 0
        summ = 0
        summ2 = 0
        
        for i1, j1 in group_k_fold(len(array), n_splits = self.n_folds, seed = seed):
            for i in range(0, array[i1].shape[1]):
                for j in range(0, len(array[i1])):
                    tmp_ind = i1[j]
                    for k in range(0, len(array[j1])):
                        summ += int(array[j1][k][i] == array[i1][j][i])
                        summ2 += np.array(Y[j1])[k] * int(array[j1][k][i] == array[i1][j][i])
                    self.array[tmp_ind][index] = summ2 / summ
                    self.array[tmp_ind][index + 1] = summ / len(array[j1])
                    summ = 0
                    summ2 = 0
                    tmp_ind = i1[j]
                index += 3
            index = 0
            
    def transform(self, X, a=1e-5, b=1e-5):
        """
        param X: objects to transform, pandas-dataframe, shape [n_objects, n_features]
        param a: constant for counters, float
        param b: constant for counters, float
        returns: transformed objects, numpy-array, shape [n_objects, 3]
        """
        result = self.array.copy()
        for i in range(0, len(self.array)):
            for j in range(2, self.array.shape[1], 3):
                result[i][j] = (self.array[i][j - 2] + a)
                result[i][j] /= (self.array[i][j - 1] + b)
        return result
        
    def fit_transform(self, X, Y, a=1e-5, b=1e-5):
        self.fit(X, Y)
        return self.transform(X, a, b)
 
       
def weights(x, y):
    """
    param x: training set of one feature, numpy-array, shape [n_objects,]
    param y: target for training objects, numpy-array, shape [n_objects,]
    returns: optimal weights, numpy-array, shape [|x unique values|,]
    """
    uniq_list = np.unique(x)
    np.unique(x)
    tmp_list = np.bincount(x)
    result = []
    summ = 0
    for a in range(0, len(tmp_list)):
        for b in range(0, len(y)):
            t = int(uniq_list[a] == x[b])
            print(t)
            summ = summ + int(y[b] and t)
        t = summ / tmp_list[a]
        result.append(t)
        summ = 0
    return np.array(result)
