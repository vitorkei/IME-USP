8516250

import numpy as np

n = [1, 21, 42]
s = [[1, 5, 21], [3, 42, 7], [12, 17, 0]]

def normal_equation_prediction(X, y):
    ones = np.ones((len(X), 1))
    step1 = np.hstack((ones, X)) # adds column of 1s
    XT = np.transpose(step1)
    step2 = np.dot(XT, step1)    # X^T * X
    step3 = np.linalg.inv(step2) # (X^T * X)^{-1}
    step4 = np.dot(step3, XT)    # (X^T * X)^{-1} * X^T
    w = np.dot(step4, y)         # w = (X^T * X)^{-1} * X^T * y
    return (np.dot(step1, w))

normal_equation_prediction(s, n)
