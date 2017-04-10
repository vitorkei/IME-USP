% f = função
% derf = derivada de f
% x0 = valor inicial para o método de newton
function root = newton(f, x0)
  global err = 0.0000001;
  % valor = f(x0)
  valor = polyval(f, x0);
  % der = f'(x0)
  der = polyval(polyder(f), x0);

  for iter = 0 : 100
    % Retorna a raíz caso tenha convergido o suficiente
    if (abs(valor) < err)
      root = x0;
      return;
    endif

    % Verifica se não haverá divisão por zero
    if (der == 0)
      root = NaN;
      return;
    endif

    % Efetua o método de Newton
    x0 = x0 - valor / der;
    valor = polyval(f, x0);
    der = polyval(polyder(f), x0);
  endfor

  root = NaN;
  return;
endfunction
