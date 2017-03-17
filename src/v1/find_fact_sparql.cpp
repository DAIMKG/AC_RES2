/*find common attributes 和这些 attributes相关的频率*/

#include<iostream>
#include<fstream>
#include<string>
#include<map>

using namespace std;

// argv[1]: 原始输入文件
//3 <Barack_Obama> <United_States> <Michelle_Obama>
//3 <Xi_Jinping> <China> <Peng_Liyuan>
// 生成的sparql: 找到common typpe， 和 type／sp／po的频率
// 现在的数据库里增加了: 
/*
<type_name> <has_count> <freq>
<s_p> <has_count> <freq>
<p_o> <has_count> <freq>
*/

int main(int argc, char* argv[])
{
	ifstream  fin(argv[1]);
	ofstream fout_fact(argv[2]);
	//ofstream fout_fact(argv[3]);

	string input[10][10];
	int row = 0;
	int col;

	string prefix1 = "PREFIX rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n";
	string prefix2 = "PREFIX base: <http://yago-knowledge.org/resource/>\n";

	while (fin >> col)
	{
		for (int i = 0; i < col; i++)
		{
			string temp;
			fin >> temp;
			input[row][i] = temp;
		}
		row++;
	}
    
    // fin common type and frequency
	fout_fact << prefix1;
	fout_fact << prefix2;
	fout_fact << "SELECT ";
	for (int i = 0; i < col; i++)
	{
		fout_fact << "?p" << i-0 << "0 ?o" << i-0 << "0 ";
		fout_fact << "?s" << i-0 << "1 ?p" << i-0 << "1 ";
	}
	fout_fact << "WHERE {" << endl;
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			string temp1 = input[j][i].substr(1,input[j][i].length() - 2);
			fout_fact << "base:" << temp1 << " ?p" << i-0 << "0 ?o" << i-0 <<"0" <<"\t" << "." << endl;
		}
		//fout_fact <<"?type" << i-0 << " base:has_count ?count" << i-0 << "\t" << "." << endl;
		// attention here: (s,p,*) part of the pattern should be put into the optional part of the sparql
		// not very precise, but I think it is useful.
		// more troublesome when parsing the results

		/* assume that an entity must have(entity, p, o), but not necessarilu\y has (s, p, entity);
		 so we use optional for (s,p,entity) part
		if any bug is found later, it can be changed easily.*/
		 
		fout_fact << "OPTIONAL {\n";
		for (int j = 0; j < row; j++)
		{
			string temp1 = input[j][i].substr(1,input[j][i].length() - 2);
			fout_fact << "?s" << i-0 << "1 ?p" << i-0 <<"1" << " base:" << temp1 <<"\t" << "." << endl;
		}
		fout_fact << "}\n";
		//fout_fact <<"?type" << i-0 << " base:has_count ?count" << i-0 << "\t" << "." << endl;
	}
	fout_fact <<"}";
	fout_fact.close();

	return 0;
}