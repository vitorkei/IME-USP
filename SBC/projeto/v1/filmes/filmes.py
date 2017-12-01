import readline

def main():
  my_list = ["bill-murray.txt", "frances-mcdormand.txt", "harvey-keitel.txt", "quentin-tarantino.txt", "uma-thurman.txt", "wes-anderson.txt"]
  my_people = ["Bill Murray", "Frances McDormand", "Harvey Keitel", "Quentin Tarantino", "Uma Thurman", "Wes Anderson"]
  movies = {}
  
  for i in range (6):
    file = my_list[i]
    with open(file) as f:
      lines = f.readlines()
    for line in lines:
      movie = line.partition(" (")[0]
      if not movie in movies:
        movies[movie] = [my_people[i]]
      else:
        if not my_people[i] in movies[movie]:
          movies[movie].append(my_people[i])

  for m in movies:
    if '\n' in m:
      continue
    print m, " - ", movies[m]
  
main()
