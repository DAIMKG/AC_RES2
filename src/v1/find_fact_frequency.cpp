#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<set>

using namespace std;
int main(int argc, char* argv[])
{
	// ./find_fact_frequency fact_out_parsed4 input_2.txt sparql_find_fact_f common_facts
	// 
	//input: output of parse_fact_csv.py
	//output :  a sparql file to find the frequency of common facts
	ifstream fin(argv[1]); // fact_out_parsed
	ifstream fin2(argv[2]); // input_2.txt 告诉程序有多少列
	ofstream fout(argv[3]); // sparql_find_fact_f
	ofstream fout2(argv[4]); //输出无序的common_facts，和输出的频率的sparql查询结果是一一对应的 
	set<string>col_sp[10]; // 最多支持10列
	set<string>col_po[10];
	set<string>::iterator it;

	int num_column;
	string s, s1,s2,s3;
	fin2 >> num_column;

	int line = 0;

	while (fin >> s >> s1 >> s2 >> s3)  // (p,o),(s,p)
	{
		if (s != "<>")
		{
			col_po[0].insert(s+" "+s1);
		}
		if (s2 != "<>")
		{	
			col_sp[0].insert(s2+" "+s3);
		}
		for (int i = 4; i < 4 * num_column; i=i+4)
		{
			fin >> s >> s1;
			if (s != "<>")
			{
				//string temp = s.substr(0,s.length()-1)+"_"+s1.substr(1,s1.length());
				col_po[i/4].insert(s+" "+s1);
			}
			fin >> s2 >> s3;
			if (s2 != "<>")
			{	

				col_sp[i/4].insert(s2+" "+s3);
			}
		}	
		line++;
	}
	for (int i = 0; i < num_column; i++)
	{
		fout2 << col_sp[i].size() + col_po[i].size() << endl;
		cout << col_sp[i].size() + col_po[i].size() << endl;
		for (it = col_po[i].begin(); it != col_po[i].end(); it++)
		{
			fout2 << *it << endl;
		}
		for (it = col_sp[i].begin(); it != col_sp[i].end(); it++)
		{
			string temp = *it;
			istringstream iss(temp);
			string s1, p1;
			iss >> s1 >> p1;
			string literal = "<__"+p1.substr(1) + " "+s1;
			fout2 << literal << endl;
		}
	}


	string prefix1 = "PREFIX rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n";
	string prefix2 = "PREFIX base: <http://yago-knowledge.org/resource/>\n";

	fout << prefix1;
	fout << prefix2;

	fout << "SELECT ";
	for (int i = 0; i < num_column; i++)
	{
		int num_po = col_po[i].size();
		int num_sp = col_sp[i].size();
		for (int j = 0; j < num_po; j++)
			fout << "?count"<< i << 0 << j << " ";
		for (int j = 0; j < num_sp; j++)
			fout << "?count" << i << 1 << j << " ";
	}
	// 最后结果只有一行，就是我要找的数量

	fout << "WHERE {\n";
	
	for (int i = 0; i < num_column; i++)
	{
		int k = 0;
		for (it = col_po[i].begin(); it != col_po[i].end(); it++)
		{
			string temp = *it;
			istringstream iss(temp);
			string p1, o1;
			iss >> p1 >> o1;
			string literal;
			literal = p1.substr(1, p1.length() - 2)+"_"+o1.substr(1, o1.length() - 2);
			fout << "OPTIONAL {\n";
			fout << "base:"<< literal << " base:has_count" << " ?count" << i << 0 << k << "\t" << ".\n";
			fout << "}\n";
			k++;
		}
		k = 0;
		for (it = col_sp[i].begin(); it != col_sp[i].end(); it++)
		{
			string temp = *it;
			istringstream iss(temp);
			string s1, p1;
			iss >> s1 >> p1;
			string literal;
			literal = s1.substr(1, s1.length() - 2)+"_"+p1.substr(1, p1.length() - 2);
			fout << "OPTIONAL {\n";
			fout << "base:"<< literal << " base:has_count" << " ?count" << i << 1 << k << "\t" << ".\n";
			fout << "}\n";
			k++;
		}
	}
	fout << "}";
	fout.close();
	return 0;
}