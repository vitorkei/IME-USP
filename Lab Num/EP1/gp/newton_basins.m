function newton_basins (f, n)
	global tol = 0.00001;
	interval = n/50;
	countx = -n;
	county = -n;
	pixelx = 0;
	pixely = 0;
	raiz_v = [];
  file = fopen("output.txt", "w");
	while (county < n) %parte imaginaria do numero
    while (countx < n) %parte real do numero
      id = 0;
      raiz = newton(f, polyder(f), countx + county * i);
      if (isnan(raiz))
        fprintf(file,"%d %d %d\n", pixely, pixelx, id)
      	countx = countx + interval;
        pixelx = pixelx +1;
        continue
      endif
      vlength = length(raiz_v);
      if (vlength == 0)
        id = 1;
        raiz_v(1) = raiz;
      elseif (vlength > 0)
        count = 1;
        repeat = 0;
        while(count <= vlength)
      	  if (abs(raiz_v(count) - raiz) < tol)
            id = count;
      	    repeat = 1;
      		  break;
          endif
      		count = count + 1;
        endwhile
      	if (repeat == 0)
      		id = vlength + 1;
      		raiz_v(vlength + 1) = raiz;
      	endif
      endif
      fprintf(file,"%d %d %d\n", pixely, pixelx, id)
      countx = countx + interval;
      pixelx = pixelx +1;
    endwhile
    countx = -n;
    pixelx = 0;
    county = county + interval;
    pixely = pixely + 1;
  endwhile
  fclose(file);
endfunction


function raiz = newton (f, derf, x0)
  global tol;
  iteration = 0;
  valor = polyval(f, x0);
  der = polyval(derf, x0);
  
  while (iteration < 100)
  	if (abs(valor) < tol)
  		raiz = x0;
  		return;
  	endif
    if (der == 0)
      raiz = NaN;
      return;
    endif
    x0 = x0 - valor / der;
    valor = polyval(f, x0);
    der = polyval(derf, x0);
    iteration = iteration + 1;
  endwhile
  
  raiz = NaN;
  return; 
endfunction
