%{

MAC0210 - Laboratório de Métodos Numéricos

         Nome           |   NUSP
Vítor Kei Taira Tamada  | 8516250
André Ferrari Moukarzel | 9298166

Exercício-programa 2 - Parte 3-3: Cálculo de erro

%}

function calculateError (originalImg, decompressedImg)
  % Considera que a imagem original e a imagem comprimida-descomprimida tem as mesmas dimensões e que são quadradas
  oIMG = imread(originalImg);
  dIMG = imread(decompressedImg);
  p = rows(originalImg);
  err = 0;
  
  for c = 1 : 3
  
    % Constroi a matriz correspondente a cor c, sendo:
    % (c == 1) : R
    % (c == 2) : G
    % (c == 3) : B
    orig(:, :) = double(oIMG(:, :, c));
    dec(:, :) = double(dIMG(:, :, c));
    % errC == errR se C == 1
    % errC == errG se C == 2
    % errC == errB se C == 3
    errC = norm((orig - dec)) / norm(orig);
    err = err + errC;
  endfor

  err = err / 3

endfunction
