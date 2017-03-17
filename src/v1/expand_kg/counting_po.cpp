#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<cstring>
#include<vector>

using namespace std;

map<int,string>id_to_po;
map<string,vector<string> >po_to_f;
int main()
{
	ifstream fin("/users/serena/desktop/_data/yagoAll_clean.ttl");
	ofstream fout("/Users/serena/desktop/_data/statis_po2.txt");
	string s, p, o, ch;
	int id = 0;
	while (fin >> s >> p >> o >> ch)
	{
		string temp = p+" "+o;
		cout << id << endl;
		//cout << temp << endl;
		cout << s << " " << p <<" "<< o <<" "<< ch << endl;
		if (po_to_f.count(temp) == 0)
		{
			id_to_po[id] = temp;
			id++;
			po_to_f[temp].push_back(s);
		}
		else
			po_to_f[temp].push_back(s);
	}
	for (int i = 0; i < id; i++)
	{
		string temp2 = id_to_po[i];
		fout << temp2 << " " << (po_to_f[temp2]).size() << endl;
	}
	return 0;
}