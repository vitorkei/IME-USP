%{

MAC0210 - Laboratório de Métodos Numéricos

         Nome           |   NUSP
Vítor Kei Taira Tamada  | 8516250
André Ferrari Moukarzel | 9298166

Exercício-programa 2 - Parte 3-1: Compressão de imagem

%}

function compress (originalImg, k)
  img = imread(originalImg);

  % Considera que a imagem dada é quadrada. Logo, rows(I) == columns(I), então qualquer uma das duas funções serve
  p = rows(img);
  
  % Percorre a imagem original, fazendo a imagem nova só receber o pixel se respeitar a forma especificada pelo enunciado
  pixelx = 1;
  for i = 1 : p
    pixely = 1;
    if (mod(i, (k + 1)) == 1)
      for j = 1 : p
        if (mod(j, (k + 1)) == 1)
        
          m(pixelx, pixely, :) = img(i, j, :);

          pixely = pixely + 1;
        endif
      endfor
      pixelx = pixelx + 1;
    endif
  endfor
  
  imwrite(m, "compressed.png");
endfunction
