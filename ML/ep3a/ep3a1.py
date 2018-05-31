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
    step1 = c_np / (a_np + b_np)
    step2 = a_np + b_np
    step3 = step2 * step2
    step4 = a_np*c_np/step3
    user_grad = step1 - step4
    
    print("auto_grad =", auto_grad, "\ntype(auto_grad) =", type(auto_grad))
    print("\n\n")
    print("user_grad =", user_grad, "\ntype(user_grad) =", type(user_grad))
    
    # END YOUR CODE
    
    return f, auto_grad, user_grad
