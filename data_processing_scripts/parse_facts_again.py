import sys
#input: common_facts
#output: common_facts_parsed
fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')


#filter out all the sp,po containing types

f = open(sys.argv[1])  
while True:  
    line = f.readline()  
    if line:  
        if (line2.find("-") == -1):
        			fout.write(line2)

    else:  
        break  
f.close() 
