# -*- coding: utf-8 -*-
import sys
#input: csv file of common facts
#output: an intermeidate file
fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')
#fout: somewhat cleaned result ...
#python parse_fact_csv.py /Users/serena/Documents/workspace/QueryRDF/fact_out fact_out_parsed


cnt = 0
num_column = 0

#filter out all the sp,po containing types

for line in open(sys.argv[1]):
	if (cnt == 0):
		cnt = cnt+1
		line = line.split(",")
		num_column = len(line) / 4
	else:
		if (line.find("#type") == -1 and line.find("\"http://") == -1 and line.find("\"") == -1):
			mark = 0
			line = line.split(",")
			for i in range(0, len(line)):
				line[i] = line[i][line[i].rfind("/") + 1:len(line[i])]
				if (line[i].find("–") != -1 or line[i].find("(") != -1):
					mark = 1
			if mark == 0:
				for i in range(0, len(line)):
						if (line[i] == '\r\n'):
							line[i] = ''
						if (line[i] != ''):
							line[i] = line[i][line[i].rfind("/") + 1:len(line[i])]
						line[i] = '<'+line[i]+ '>'
						if (i < num_column * 4-1):
							fout.write(line[i]+" ")
						else:
							fout.write(line[i])	
			fout.write("\n")
fout.close()

