
function roman_num (num)
  if num >= 1 and num <= 3999 then

    -- Recebe cada algarismo do número
    uni = math.floor(num % 10)
    num = num / 10

    dez = math.floor(num % 10)
    num = num / 10

    cen = math.floor(num % 10)
    num = num / 10

    mil = math.floor(num % 10)
    num = num / 10

--[[
    print ()
    print ("mil = " .. mil)
    print ("cen = " .. cen)
    print ("dez = " .. dez)
    print ("uni = " .. uni)
--]]
    print ()
    io.write("output = ")

    -- Imprime a parte de milhar
    for i = 1, mil do
      io.write("M")
    end

    -- Imprime a parte da centena
    if     cen == 4 then io.write ("CD")
    elseif cen == 9 then io.write ("CM")
    else
      if cen >= 5 then
        cen = cen - 5
        io.write("D")
      end
      for i = 1, cen do
        io.write("C")
      end
    end

    -- Imprime a parte da dezena
    if     dez == 4 then io.write("XL")
    elseif dez == 9 then io.write("XC")
    else
      if dez >= 5 then
        dez = dez - 5
        io.write("L")
      end
      for i = 1, dez do
        io.write("X")
      end
    end

    -- Imprime a parte da unidade
    if uni == 4 then io.write("IV")
    elseif uni == 9 then io.write("IX")
    else
      if uni >= 5 then
        uni = uni - 5
        io.write("V")
      end
      for i = 1, uni do
        io.write("I")
      end
    end

    print ("\n")

  else
    print ("Número fora do intervalo especificado")
  end
end

io.write ("Insira um número pertencente ao intervalo [1, 3999] para ele ser convertido em um número romano\nnum = ")

num = io.read("*n")
roman_num(num)

--[[
for i = 1, 3999 do
  roman_num(i)
end
--]]

