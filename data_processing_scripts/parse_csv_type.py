import sys

#input: sparql_file_name+_out.csv
#output: all_ranked_types
# output format:
# #(same types) for the first column
# most common types
# second most common types
#
# #(same types) for the second column
# most common types
# second most common types
#......

fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')
fin2 = open(sys.argv[2],'r')
fout2 = open(sys.argv[3],'w')

cnt = 0
num_column = 0

for line in open(sys.argv[1]):
	if (cnt == 0):
		cnt = cnt+1
		line = line.split(",")
		num_column = len(line) / 2
	else:
		line = line.split(",")
		for i in range(0, num_column * 2):
			line[i] = line[i][line[i].rfind("/") + 1:len(line[i])]
			if i % 2 == 0:
				line[i] = '<'+line[i]+ '>'
			if (i < num_column*2-1):
				fout.write(line[i]+" ")
			else:
				fout.write(line[i])	
fout.close()
		#fout.write("\n")
d = list()		
d.append(dict())
d.append(dict())
d.append(dict())
d.append(dict())
d.append(dict())
d.append(dict())
d.append(dict())
d.append(dict())
# at most 7 columns

for line2 in open(sys.argv[2]):
	line2 = line2.split(" ")
	for i in range(0, num_column*2-2, 2):
		d[i/2][line2[i]] = int(line2[i+1])
	d[num_column-1][line2[num_column*2-2]] = int(line2[num_column*2-1][0:len(line2[num_column*2-1])-2])

print("num column is: %d\n", num_column)
for i in range(0, num_column):
	dd = d[i]
	print  sorted(dd.items(), key = lambda dd:dd[1], reverse = True)
	ddd = sorted(dd.items(), key = lambda dd:dd[1], reverse = True)
	fout2.write(str(len(ddd)))
	fout2.write("\n")
	#print("\n")
	#print dd
	for key, value in ddd:
		fout2.write(key + "\n")


