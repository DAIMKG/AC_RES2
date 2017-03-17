#include<iostream>
#include<ifstream>
#include<map>
#include<strig>
#include<cstring>

using namespace std;

map<int,string>id_to_sp;
map<string,vector<string> >sp_to_f;
int main()
{
	ifstream fin("/users/serena/desktop/yagoAll_clean.ttl");
	ofstream fout("/Users/serena/Documents/workspace/QueryRDF/statis_sp.txt");
	string s, p, o, ch;
	int id = 0;
	while (fin >> s >> p >> o >> ch)
	{
		string temp = s+" "+p;
		if (sp_to_f.count(temp) == 0)
		{
			id_to_sp[id] = sp;
			id++;
			sp_to_f[temp].push_back(o);
		}
		else
			sp_to_f[temp].push_back(o);
	}
	for (int i = 0; i < id; i++)
	{
		string temp2 = id_to_sp[i];
		fout << temp2 << " " << (sp_to_f[temp2]).size() << endl;
	}
	return 0;
}