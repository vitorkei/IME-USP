p(X, [X | Y]).
p(X, [Y | Z]) :- p(X, Z).

