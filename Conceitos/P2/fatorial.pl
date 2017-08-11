
fatorial(A,B) :-
		   A > 0,
           C is A-1,
           fatorial(C,D),
           B is A*D.

fatorial(2,2).
