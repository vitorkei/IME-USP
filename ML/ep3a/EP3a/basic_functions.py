import torch.nn.functional as F
import numpy as np
import torch
from util import randomize_in_place


def sigmoid(x):
    return 1 / (1 + np.exp(-x))


def graph1(a_np, b_np, c_np):
    """
    Computes the graph
        - x = a * c
        - y = a + b
        - f = x / y

    Computes also df/da using
        - Pytorchs's automatic differentiation (auto_grad)
        - user's implementation of the gradient (user_grad)

    :param a_np: input variable a
    :type a_np: np.ndarray(shape=(1,), dtype=float64)
    :param b_np: input variable b
    :type b_np: np.ndarray(shape=(1,), dtype=float64)
    :param c_np: input variable c
    :type c_np: np.ndarray(shape=(1,), dtype=float64)
    :return: f, auto_grad, user_grad
    :rtype: torch.DoubleTensor(shape=[1]),
            torch.DoubleTensor(shape=[1]),
            numpy.float64
    """
    # YOUR CODE HERE:
    a = torch.from_numpy(a_np)
    b = torch.from_numpy(b_np)
    c = torch.from_numpy(c_np)
    a.requires_grad = True
    
    x = a * c
    y = a + b
    f = x / y
    
    f.backward()
    auto_grad = a.grad
    
    # df/da = c/(a+b) - (a*c/(a+b)^2)
    step1 = a_np + b_np
    step2 = c_np / step1
    step3 = step1 * step1
    step4 = a_np * c_np / step3
    user_grad = step2 - step4
    # END YOUR CODE
    return f, auto_grad, user_grad


def graph2(W_np, x_np, b_np):
    """
    Computes the graph
        - u = Wx + b
        - g = sigmoid(u)
        - f = sum(g)

    Computes also df/dW using
        - pytorchs's automatic differentiation (auto_grad)
        - user's own manual differentiation (user_grad)

    F.sigmoid may be useful here

    :param W_np: input variable W
    :type W_np: np.ndarray(shape=(d,d), dtype=float64)
    :param x_np: input variable x
    :type x_np: np.ndarray(shape=(d,1), dtype=float64)
    :param b_np: input variable b
    :type b_np: np.ndarray(shape=(d,1), dtype=float64)
    :return: f, auto_grad, user_grad
    :rtype: torch.DoubleTensor(shape=[1]),
            torch.DoubleTensor(shape=[d, d]),
            np.ndarray(shape=(d,d), dtype=float64)
    """
    # YOUR CODE HERE:
    W = torch.from_numpy(W_np)
    x = torch.from_numpy(x_np)
    b = torch.from_numpy(b_np)
    W.requires_grad = True
    
    u = torch.matmul(W, x) + b
    g = F.sigmoid(u)
    f = torch.sum(g)
    
    f.backward()
    auto_grad = W.grad
    
    # df/dW = df/dg * dg/du * du/dW
    # du/dW = 1 * x + 0 = x
    # dg/du = g * (1 - g)
    # df/dg = 1
    dudW = x_np
    g2 = sigmoid(np.matmul(W_np, x_np) + b_np)
    dgdu = g2 * (1 - g2)
    
    user_grad = np.matmul(dgdu, np.transpose(dudW))

    # END YOUR CODE
    return f, auto_grad, user_grad


def SGD_with_momentum(X,
                      y,
                      inital_w,
                      iterations,
                      batch_size,
                      learning_rate,
                      momentum):
    """
    Performs batch gradient descent optimization using momentum.

    :param X: design matrix
    :type X: np.ndarray(shape=(N, d))
    :param y: regression targets
    :type y: np.ndarray(shape=(N, 1))
    :param inital_w: initial weights
    :type inital_w: np.array(shape=(d, 1))
    :param iterations: number of iterations
    :type iterations: int
    :param batch_size: size of the minibatch
    :type batch_size: int
    :param learning_rate: learning rate
    :type learning_rate: float
    :param momentum: accelerate parameter
    :type momentum: float
    :return: weights, weights history, cost history
    :rtype: np.array(shape=(d, 1)), list, list
    """
    # YOUR CODE HERE:
    w = torch.from_numpy(inital_w)
    w.requires_grad = True

    weights_history = []
    cost_history = []
    z = 0

    for t in range(iterations):
        weights_history.append(w.clone())

        # gera um minibatch de batch_size elementos escolhidos aleatoriamente
        Xm = []
        ym = []
        ids = np.random.choice(np.arange(X.shape[0]), batch_size, replace=False)
        for i in ids:
            Xm.append(X[i])
            ym.append(y[i])
        Xm = torch.DoubleTensor(Xm)
        ym = torch.DoubleTensor(ym)

        # calcula o custo (J) e o grande de J em w
        step1 = torch.matmul(Xm, w)
        step2 = step1 - ym
        step2T = torch.transpose(step2, 0, 1)
        step3 = torch.matmul(step2T, step2)
        J = step3 / batch_size
        J.backward()
        dJdw = w.grad

        z = momentum * z + dJdw
        w.data -= learning_rate * z
        cost_history.append(J[0][0])
        w.grad.zero_()

    w_np = w.detach().numpy()
    # END YOUR CODE

    return w_np, weights_history, cost_history
