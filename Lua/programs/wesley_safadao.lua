function anjo_safadao (d, m, a)
  safadeza = m + a/100 * (50 - d)
  anjo = 100 - safadeza
  print ("Safadeza: " .. safadeza .. "%")
  print ("Anjo: " .. anjo .. "%")
end

print ("Insira o dia do nascimento: " .. d)
d = io.read()

print ("Insira o mês do nascimento: ")
mes = io.read()

print ("Insira o ano do nascimento: ")
ano = io.read()

anjo_safadao (dia, mes, ano)