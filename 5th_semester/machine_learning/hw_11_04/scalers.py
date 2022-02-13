import numpy as np

class MinMaxScaler():
    def __init__(self):
        self.min = 0
        self.max = 1

    def fit(self, data):
        """Store calculated statistics

        Parameters:
        data (np.array): train set, size (num_obj, num_features)
        """
        self.min = np.min(data, axis=0)
        self.max = np.max(data, axis=0)
        
    def transform(self, data):
        """
        Parameters:
        data (np.array): train set, size (num_obj, num_features)

        Return:
        np.array: scaled data, size (num_obj, num_features)
        """
        return (data - self.min) / (self.max - self.min)


class StandardScaler():
    def __init__(self):
        self.std = 1
        self.mean_val = 0

    def fit(self, data):
        """Store calculated statistics
        
        Parameters:
        data (np.array): train set, size (num_obj, num_features)
        """
        self.mean_val = np.mean(data, axis=0)
        self.std = np.std(data, axis=0)
        
    def transform(self, data):
        """
        Parameters:
        data (np.array): train set, size (num_obj, num_features)

        Return:
        np.array: scaled data, size (num_obj, num_features)
        """
        return (data - self.mean_val) / self.std
