# 8516250

import numpy as np

def normal_equation_prediction(X, y):
    """
    Calculates the prediction using the normal equation method.
    You should add a new column with 1s.

    :param X: design matrix
    :type X: np.array
    :param y: regression targets
    :type y: np.array
    :return: prediction
    :rtype: np.array
    """
    ones = np.ones((len(X), 1))
    step1 = np.hstack((ones, X)) # adds column of 1s
    XT = np.transpose(step1)
    step2 = np.dot(XT, step1)    # X^T * X
    step3 = np.linalg.inv(step2) # (X^T * X)^{-1}
    step4 = np.dot(step3, XT)    # (X^T * X)^{-1} * X^T
    w = np.dot(step4, y)         # w = (X^T * X)^{-1} * X^T * y
    
    return (np.dot(step1, w))
