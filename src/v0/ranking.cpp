#include<iostream>
#include<fstream>
#include<map>
#include<cmath>
#include<string>
#include<vector>

#define ENTITY 1000

using namespace std;

map<string,long>entity_to_id;
map<long,string>id_to_entity;
vector<pair<int,double> >line[ENTITY];
//map<int,double>::iterator iter;

int main(char* argv[])
{
	//entity_map_result.txt
	ifstream fin(argv[1]);
	ifstream fin1("entity_map_result.txt");
	string s; long index;
	string sub, pred, obj;
	while(fin1 >> s >> index)
	{
		entity_to_id[s] = index;
		id_to_entity[index] = s;
	}

	double page_rank[ENTITY];
	double page_rank2[ENTITY];

	for (int i = 0; i < ENTITY; i++)
	{
		page_rank[i] = 1;
		page_rank2[i] = 0.1;
	}

	while(fin >> sub >> pred >> obj)
	{
		line[entity_to_id[sub]].push_back(std::make_pair(entity_to_id[obj],1)); // 只记录不为0 的标号和对应的值
	}   

	for (int i = 0; i < ENTITY; i++)
	{
		int sum = line[i].size(); // 有多少个不为0的
		
		for (int j = 0; j < sum; j++)
		{
			line[i][j].second = 1.0 / sum;
		}
	} 

    int iteration = 0;
    while (iter < 50)  
    {
    	for (int i = 0; i < ENTITY; i++)
    	{
    		page_rank[i] = page_rank2[i];
    	}
    	iter++;  // 表示迭代轮数
    	cout << iter << endl;
    	for (int i = 0; i < ENTITY; i++)
    		page_rank2[i] = 0;
    	for (int i = 0; i < ENTITY; i++)
    	{
    		int sum = line[i].size();
    		for (int j = 0; j < sum; j++)
    		{
    			page_rank2[line[i][j].first] += (line[i][j].second)*page_rank[i]*0.85;
    		}
    		page_rank2[i] += 0.15 / ENTITY* 1 ;
    	}
    	//cout << mapper1[1] << " " << page_rank[1] << endl;
    }
    for (int i = 0; i < ENTITY; i++)
    {
        page_rank[i] = page_rank2[i];
    }

	return 0;
}


