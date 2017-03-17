import sys

# argv[1]: common_facts
# argv[2]: sparql_find_fact_f_out.csv  total: 2 lines !
# argv[3]: all ranked attributes(the same format as ranked_facts_2.txt)
# local usage: python rank_attributes.py common_facts sparql_find_fact_f_out.csv ranked_facts_using_sparql

fin = open(sys.argv[1],'r')
fin2 = open(sys.argv[2],'r')
fout = open(sys.argv[3],'w')

cnt = 0;
line = fin2.readline()
line = fin2.readline()


line = line.split(",")
print line
index = dict()
for i in range (0, len(line)):
	if line[i] != '':
		index[i] = int(line[i][line[i].rfind("/")+1:len(line[i])])
		print index[i]
	else:
		index[i] = int(0)
fin2.close()

while True:  
    line = fin.readline()  
    if line:  
    	temp_line = line
        if len(temp_line.split(" ")) == 1:
        	fout.write(line)
        	d = dict()
        	for i in range(0,int(line)):
        		line2 = fin.readline()
        		d[line2] = index[cnt]
        		cnt = cnt+1
        	dd = sorted(d.items(), key = lambda d:d[1], reverse = True)
        	for key, value in dd:
				fout.write(key)

    else:  
        break  
fin.close() 
