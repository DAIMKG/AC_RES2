import sys

fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')

for line in open(sys.argv[1]):
	line = line.split(" ")
	if (line[1] != '.'):
		fout.write(line[0]+" "+line[1]+" "+line[2])