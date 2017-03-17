import sys

fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')
#python forming_po_rdf.py /users/serena/desktop/_data/yagoAll_clean.ttl sp_rdf_new

cnt = 11
dic = dict()

while True:  
    line = fin.readline()  
    if line:  
        if cnt > 0:
        	cnt = cnt - 1
        if (line.find("type") == -1 and len(line.split("/t")) == 4):
        	literal = line[1][0:len(line[0])-1]+"_"+line[2][1:len(line[2])]
        	if literal in dic.keys():
        		dic[literal] = dic[literal] +1
        	else:
        		dic[literal] = 1

    else:  
        break  
f.close() 

for key, value in dic:
	fout.write(key + " " + "<has_count>" + " <" + str(value) + ">\n")
