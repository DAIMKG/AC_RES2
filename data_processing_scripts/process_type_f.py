import sys

fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')

for line in open(sys.argv[1]):
	tmp = line.rfind(",")
	length = len(line)
	str3 = line[0:tmp]
	tmp2 = str3.rfind("/")
	str1 = str3[tmp2+1:len(str3)]
	if str1[len(str1)-1] == "\"":
		str1 = str1[0:len(str1)-1]
	str2 = line[tmp+1:length]
	fout.write("<"+str1+">"+" "+str2)
