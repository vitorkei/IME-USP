function newton_basins (f, n)
  err = 0.0000001;
  file = open("output.txt", "w");
  range = 1;
  roots = []; % vetor que armazena as raízes da função
  
  % (x, y) corresponde a um pixel de coordenada (x, y) na imagem gerada
  for y =  0 : (n - 1)
    for x =  0 : (n - 1)
      % c = color; Determina qual cor será imprimida
      c = 1;
      % Encontra a raíz da função no ponto (x, y) pelo método de Newton
      root = newton(f, polyder(f), x + y * i);
      
      % Verifica se a raíz é um número inválido e 
      if (isnan(root))
        fprintf(file, "%d %d %d\n", x, y, 0);
        continue
      endif
      
      i = length(roots);
      
    endfor
  endfor
  
endfunction
