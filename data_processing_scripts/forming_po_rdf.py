import sys

fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')
#python forming_po_rdf.py /users/serena/desktop/_data/yagoAll_clean.ttl po_rdf_new

cnt = 11
dic = dict()
line_num = 0

while True:  
    line = fin.readline()  
    if line:  
        if cnt > 0:
        	cnt = cnt - 1
        if (line.find("type") == -1):
        	line = line.split("\t")
        	if (len(line) == 3):
        		line_num = line_num+1
        		if (line_num % 100000 == 0):
        			print (line_num)
        		ll = line[2].split(" ")
        		literal = line[1][0:len(line[1])-1]+"_"+ll[0][1:len(ll[0])]
        		if literal in dic.keys():
        			dic[literal] = dic[literal] +1
        		else:
        			dic[literal] = 1

    else:  
        break  
fin.close() 

for key in dic:
	fout.write(key + " " + "<has_count>" + " <" + str(dic[key]) + ">\n")
