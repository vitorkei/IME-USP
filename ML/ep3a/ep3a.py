import numpy as np
import torch

def graph1(a_np, b_np, c_np):
  x = a_np * c_np
  y = a_np + b_np

  f = torch.DoubleTensor(x / y)
  f.requires_grad = True
  fGrad = torch.sum(f)
  fGrad.backward()

  auto_grad = f.grad
  user_grad = 0

  return f, auto_grad, user_grad

for _ in range(1000):
  a_np = np.random.rand(1)
  b_np = np.random.rand(1)
  c_np = np.random.rand(1)

  f, auto_grad, user_grad = graph1(a_np, b_np, c_np)
  manual_f = (a_np * c_np) / (a_np + b_np)

  assert np.isclose(f.data.numpy()[0], manual_f, atol=1e-4), "Valor do f com problemas"
  assert np.isclose(auto_grad.numpy()[0], user_grad()), "Derivada parcial com problemas"
