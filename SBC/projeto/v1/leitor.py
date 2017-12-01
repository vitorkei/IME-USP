import readline


def main():
	file = "directors.list"
	first = "wes"
	last = "anderson"


	with open(file) as f:
		lines = f.readlines()

	found = 0
	for line in lines:
		lin = line.split()
		if not found:
			if len(lin) > 1:
				if lin[0].lower() == str(last + ',') and lin[1].lower() == first:
					print (line)
					found = 1
		else:
			if len(lin) < 1:
				break
			print(line)

main()