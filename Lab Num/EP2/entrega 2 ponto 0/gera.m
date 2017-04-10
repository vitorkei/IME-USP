function gera(size)
  for i = 1 : size
    for j = 1 : size
      OUT(i, j, 1) = sin(i)*255;
      OUT(i, j, 2) = ((sin(j) + sin(i))/2)*255;
      OUT(i, j, 3) = sin(i)*255;
    endfor
  endfor
  OUT = uint8(OUT);
  imwrite(OUT, "decompressed.png");
endfunction