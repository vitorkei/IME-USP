import torch.nn.functional as F

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
    # du/dW = 1*x + 0 = x
    # dg/du = g*(1 - g)
    # df/dg = 1
    dudW = x_np
    g2 = sigmoid(np.matmul(W_np, x_np) + b_np)
    dgdu = g2 * (1 - g2)
    
    user_grad = np.matmul(dgdu, np.transpose(dudW))
    
    # END YOUR CODE
    return f, auto_grad, user_grad
