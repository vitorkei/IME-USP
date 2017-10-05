homem(americo ).
homem(daniel ).
homem(paulo ).
homem(carlos ).
homem(joaquim ).
homem(filipe ).
mulher(teresa ).
mulher(sonia ).
mulher(ana).
mulher(carla ).
mulher(barbara ).
mulher(maria ).
idade(americo , 18).
idade(daniel , 60).
idade(paulo , 25).
idade(carlos , 37).
idade(joaquim , 80).
idade(filipe , 32).
idade(teresa , 18).
idade(sonia , 28).
idade(ana , 17).
idade(carla , 26).
idade(barbara , 51).
idade(maria , 79).
irmaos(americo , paulo ).
irmaos(carlos , sonia ).
pai(carlos , teresa ).
pai(daniel , americo ).
pai(daniel , paulo ).
pai(joaquim , daniel ).
mae(maria , daniel ).
mae(barbara , ana).
casados(filipe , carla ).
casados(americo ,teresa ).
casados(joaquim , maria ).

/* a */
avof(Mul, Pess) :- mae(Mul, X), (mae(X, Pess); pai(X, Pess)).
/*
avof(Mul, Pess) :- mae(Mul, X), mae(X, Pess).
avof(Mul, Pess) :- mae(Mul, X), pai(X, Pess).
*/

/* b */
avom(Hom, Pess) :- pai(Hom, X), (mae(X, Pess); pai(X, Pess)).
/*
avom(Hom, Pess) :- pai(Hom, X), mae(X, Pess).
avom(Hom, Pess) :- pai(Hom, X), pai(X, Pess).
*/

/* c */
bisavom(Hom, Pess) :- pai(Hom, X), (avom(X, Pess); avof(X, Pess)).
/*
bisavom(Hom, Pess) :- pai(Hom, X), avom(X, Pess).
bisavom(Hom, Pess) :- pai(Hom, X), avof(X, Pess).
*/

/* d */
primo_1(P1, P2) :- (pai(X, P1); mae(X, P1)), (pai(Y, P2); mae(Y, P2)), irmaos(X, Y).
/*
primo_1(P1, P2) :- mae(X, P1), mae(Y, P2), irmaos(X, Y).
primo_1(P1, P2) :- mae(X, P1), pai(Y, P2), irmaos(X, Y).
primo_1(P1, P2) :- pai(X, P1), mae(Y, P2), irmaos(X, Y).
primo_1(P1, P2) :- pai(X, P1), pai(Y, P2), irmaos(X, Y).
*/

/* e */

/* f */
maior_de_idade(Pess) :- idade(Pess, X), X >= 18.

/* g */
p(X) :- homem(X); mulher(X).
pessoas(Lista) :- findall(X, p(X), Lista).

/* h */
max([], Max, Max).
max([X|Rest], Y, Max) :- X > Y, max(Rest, X, Max).
max([X|Rest], Y, Max) :- X =< Y, max(Rest, Y, Max).
max([X|Rest], Max) :- max(Rest, X, Max).

lista_idades(Lista) :- findall(Idade, idade(X, Idade), Lista).

maior_idade(Max) :- lista_idades(Lista), max(Lista, Max).
m_v(Pess) :- maior_idade(Max), idade(Pess, Max).

mais_velho(Pess) :- m_v(Pess, Max).

/* i */
m(X, Idade) :- idade(X, Idade), homem(X).
f(X, Idade) :- idade(X, Idade), mulher(X).
lista_pessoas(Lista, Sexo) :-
  Sexo == 'm' -> findall([X, Idade], m(X, Idade), Lista) ;
  Sexo == 'f' -> findall([X, Idade], f(X, Idade), Lista).

/* j */
/*
adequados(Hom, Mul) :-
  homem(Hom),
  mulher(Mul),
  idade(Hom, IdadeH),
  idade(Mul, IdadeM),
  IdadeH >= IdadeM - 2,
  IdadeH <= IdadeM + 10,
*/
