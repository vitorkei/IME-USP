# notebook feito para a versão 0.4.0
import numpy as np
import torch

print("PyTorch version = {} ".format(torch.__version__))

print("tensor A\n")
A = torch.Tensor([[1., 1.], [1., 2.]])
print("type = ", A.type())
print("shape = ", A.shape)
print(A)

print("\ntensor B\n");
B = torch.Tensor([[2., 2.], [3., 4.]])
print("type = ", B.type())
print("shape = ", B.shape)
print(B)

print("\ntensor C\n")
C = torch.Tensor((4, 2, 2))
print("type = ", C.type())
print("shape = ", C.shape)
print(C)

print("\ntensor D\n")
D = torch.rand((3, 3))
print("type = ", D.type())
print("shape = ", D.shape)
print(D)

print("\n##########################\n\n")

A_flat = A.view((4, 1))
A_flat = A_flat.type(torch.ShortTensor)
print("tensor A_flat\n")
print("type = ", A_flat.type())
print("shape = ", A_flat.shape)
print(A_flat)

B_flat = B.view((4, 1))
B_flat = B_flat.type(torch.ByteTensor)
print("\ntensor B_flat\n")
print("type = ", B_flat.type())
print("shape = ", B_flat.shape)
print(B_flat)

print("\n\n")

print("A[0][0] = ", A[0][0])
print("A[0:1,:] = ", A[0:1,:])
print("A[1,:] = ", A[1,:])
print("B_flat[-1] = ", B_flat[-1])

print("\n##########################\n\n")

print("A = ", A)
print()
print("B = ", B)

print()
print("- (soma)\n A + B = \n", A+B)

print()
print("- (multiplicação por escalar)\n A * 9.2 = \n", A * 9.2)

print()
print("- (hadammar product -- multiplicação elemento a elemento)\n A * B = \n", A * B)

print()
print("- (redução por soma)\n torch.sum(A) = \n", torch.sum(A))

print()
print("- (redução por média)\n torch.mean(B) = \n", torch.mean(B))

print()
def sigmoid(x):
  return 1/(1 + np.exp(-x))
print("- (aplicando uma função escalar num tensor)\n sigmoid(A) = \n", sigmoid(A))

print("\n##########################\n\n")

x = torch.randn(5, 5)
y = torch.randn(5, 5)
z = torch.randn((5, 5), requires_grad=True)
a = x + y
b = a + z
print("a.requires_grad =", a.requires_grad)
print("b.requires_grad =", b.requires_grad)

try:
  b.backward()
  print(z.grad)
except RuntimeError as e:
  print(e)

u = torch.sum(b)
u.backward()
print("\nz.grad =\n", z.grad)

print("\n##########################\n\n")

A = torch.Tensor([[1., 1.], [1., 2.]])
B = torch.Tensor([[2., 2.], [3., 4.]])
A.requires_grad = True
C = A * B
d = torch.sum(C)
d.backward()
print("A.grad =\n", A.grad)

print("\n##########################\n\n")

try:
  A = torch.Tensor([[1., 1.], [1., 2.]])
  B = torch.Tensor([[2., 2.], [3., 4.]])
  A.requires_grad = True
  C = A * B
  d = torch.sum(C)
  for i in range(5):
    print("i={}".format(i))
    d.backward()
    print(A.grad)
    print()
except RuntimeError as e:
  print(e)

print("\n##########################\n\n")

A = torch.Tensor([[1., 1.], [1., 2.]])
B = torch.Tensor([[2., 2.], [3., 4.]])
A.requires_grad = True
C = A * B
d = torch.sum(C)
for i in range(5):
  print("i={}".format(i))
  d.backward(retain_graph=True)
  print(A.grad)

print("\n##########################\n\n")

A = torch.Tensor([[1., 1.], [1., 2.]])
B = torch.Tensor([[2., 2.], [3., 4.]])
A.requires_grad = True
C = A * B
d = torch.sum(C)
for i in range(5):
  print("i={}".format(i))
  d.backward(retain_graph=True)
  print(A.grad)
  A.grad.zero_()
