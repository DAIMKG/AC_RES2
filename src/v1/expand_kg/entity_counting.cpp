#include<iostream>
#include<fstream>
#include<map>

using namespace std;

int main(char* argv[])
{
    ///mnt/disk1/daim/simon/simon/RDFCount
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    ofstream fout2("entity_map_result.txt");
    map<string, int>mapper;
    map <string,int>::iterator iter;
    string sub, pred, obj;
    int cnt = 0;

    while(fin >> sub >> pred >> obj)
    {
    	if (mapper.count(sub) == 0)
    	{
    		fout2 << sub << "\t" << cnt << endl;
    		cnt++;
    		mapper[sub] = 1;
    	}
    	else
    		mapper[sub]++;

    	if (mapper.count(obj) == 0)
    	{
    		fout2 << obj << "\t" << cnt << endl;
    		cnt++;
    		mapper[obj] = 1;
    	}
    	else
    		mapper[obj]++;
    }

  //for (m1_Iter = m1.begin( ); m1_Iter != m1.end( ); m1_Iter++ )
    fout << "entity_num = " << cnt << endl;

    for (iter = mapper.begin(); iter != mapper.end(); iter++)
    	fout << iter->first << "\t" << iter -> second << endl;
	return 0;
}