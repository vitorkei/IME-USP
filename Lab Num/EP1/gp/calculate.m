function ret = calculate (f, x0)
	fsize = length(f) - 1; % grau da função
	soma = 0;

	while (fsize >= 0)
	  % Encontra o x0^(grau da função) %
		temp = fsize;
		xtemp = 1;
		while(temp > 0)
			xtemp = xtemp * x0;
			temp = temp - 1;
	    endwhile
	  %%%%%%%%%%%%%%%%
		soma = soma + xtemp * f([length(f) - fsize]);
		fsize = fsize - 1;
  endwhile
  ret = soma;
	return
endfunction
