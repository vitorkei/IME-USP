% metodo de newton para estimar a raiz de f(x0)
function raiz = newton (f, derf, x0)
  tol = 0.000001;
  valor = polyval(f, x0);
  der = polyval(derf, x0);
  
  while (abs(valor) > tol)
    if (der == 0)
      raiz = NaN;
      return;
    endif
    x0 = x0 - (valor / der);
    valor = polyval(f, x0);
    der = polyval(derf, x0);
  endwhile
    
  raiz = x0;
  return
endfunction
