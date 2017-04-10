# Nome: Vítor Kei Taira Tamada
# NUSP: 8516250

# When done, submit this entire file to the autograder.

# Part 1

def sum arr
  x = 0
  arr.each do |i|
    x += i
  end
  return x
end

def max_2_sum arr
  if arr.empty?
    return 0
  elsif arr.size == 1
    return arr[0]
  else
    arr = arr.sort
    l = arr.size
    a = arr[l-1] + arr[l-2]
    return a
  end
end

def sum_to_n? arr, n
  if arr.empty?
    return false
  elsif arr.size == 1
    return false
  else
    arr.each do |i|
      x = n - i
      if arr.include?(x) and x != i
        return true
      end
    end
  end
  return false
end

# Part 2

def hello(name)
  return "Hello, " + name
end

def starts_with_consonant? s
  if s.empty?
    return false
  # Verifica se é uma consoante maiúscula
  elsif s[0].ord >= 66 and s[0].ord <= 90 and s[0].ord != 69 and s[0].ord != 73 and s[0].ord != 79 and s[0].ord != 85
    return true
  # Verifica se é uma consoante minúscula
  elsif s[0].ord >= 98 and s[0].ord <= 122 and s[0].ord != 101 and s[0].ord != 105 and s[0].ord != 111 and s[0].ord != 117
    return true
  end
  return false
end

def binary_multiple_of_4? s
  if /[^01]/.match(s)
    return false
  elsif s.size == 0
    return false
  end
  l = s.size
  if s[l-1] == '1' or s[l-2] == '1'
    return false
  end
  return true
end

# Part 3

class BookInStock
  attr_reader :isbn
  attr_accessor :price
  
  def initialize(isbn, price)
    @isbn = isbn
    @price = Float(price)
    if isbn.empty?
      raise ArgumentError.new("ISBN is empty")
    elsif price <= 0.0
      raise ArgumentError.new("Price is lower or equal than 0")
    end
  end
  
  def isbn
    @isbn
  end
  def isbn=(new_isbn)
    @isbn = new_isbn
  end
  
  def price
    @price
  end
  def price=(new_price)
    @price = new_price
  end
  
  def price_as_string
    s = "$"
    p = price.to_s
    if /\d\.0/.match(p) # price no formato 21.0 por exemplo (apenas 0 depois do .)
      p = p + "0"
      s = s + p
      return s
    elsif /\d\s/.match(p) # price no formato 21 por exemplo (sem .)
      p = p + ".00"
      s = s + p
      return s
    elsif /\d\.\d\d/.match(p) # price no formato 42.21 por exemplo (dois digitos depois do .)
      s = s + p
      return s
    elsif /\d\.[^0]/.match(p) # price no formato 4.2 por exemplo (um digito diferente de zero depois do .)
      p = p + "0"
      s = s + p
      return s
    end
  end
end
