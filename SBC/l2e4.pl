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
pessoas(Lista) :- Lista = [].
pessoas(Lista) :- Lista = .(X, Lista), homem(X), !.
