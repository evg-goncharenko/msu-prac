import os

from sklearn.ensemble import ExtraTreesRegressor
from sklearn.ensemble import RandomForestRegressor
from sklearn.pipeline import Pipeline
from sklearn.model_selection import GridSearchCV

import numpy as np
import pandas as pd


class PotentialTransformer:
    def fit(self, x, y):
        return self

    def fit_transform(self, x, y):
        return self.transform(x)

    def transform(self, x):
        for j in range(len(x)):
            image = x[j].copy()
            image = pd.DataFrame(image)
            columns = []
            rows = []
            for i in range(256):
                if np.min(image.iloc[i, :]) <= 2:
                    rows.append(i)
            for i in range(256):
                if np.min(image.iloc[:, i]) <= 2:
                    columns.append(i)
            image = np.roll(image, image.shape[1] // 2 - (rows[0] + rows[-1]) // 2, axis=0)
            image = np.roll(image, image.shape[1] // 2 - (columns[0] + columns[-1]) // 2, axis=1)
            x[j] = image
        return x.reshape((x.shape[0], -1))



def load_dataset(data_dir):
    files, X, Y = [], [], []
    for file in os.listdir(data_dir):
        potential = np.load(os.path.join(data_dir, file))
        files.append(file)
        X.append(potential["data"])
        Y.append(potential["target"])
    return files, np.array(X), np.array(Y)


def train_model_and_predict(train_dir, test_dir):
    _, X_train, Y_train = load_dataset(train_dir)
    test_files, X_test, _ = load_dataset(test_dir)
    pipe = Pipeline([('vectorizer', PotentialTransformer()),
                     ('regr', ExtraTreesRegressor(n_estimators=4000, random_state=3, max_depth=80, max_features=10, warm_start=True, criterion="mae"))])
    pipe.fit(X_train, Y_train)
    predictions = pipe.predict(X_test)
    return {file: value for file, value in zip(test_files, predictions)}
