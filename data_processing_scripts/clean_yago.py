import sys
if(len(sys.argv) != 3):
	print("usage: python clean_yago.py <inputData> <outputData>")
	exit()
fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')

for line in open(sys.argv[1]):
	line = line.split("\t")
	fout.write(line[1])