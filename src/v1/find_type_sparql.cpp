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
// output: a sparql query file to find the common types and their frequencies

int main(int argc, char* argv[])
{
	ifstream  fin(argv[1]);
	ofstream fout_type(argv[2]);
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
	fout_type << prefix1;
	fout_type << prefix2;
	fout_type << "SELECT ";
	for (int i = 0; i < col; i++)
	{
		fout_type << "?";
		fout_type << "type" << i-0 << " ?count" << i-0 << " ";
	}
	fout_type << "WHERE {" << endl;
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			string temp1 = input[j][i].substr(1,input[j][i].length() - 2);
			fout_type << "base:" << temp1 << " rdf:type ?type" << i-0 << "\t" << "." << endl;
		}
		fout_type <<"?type" << i-0 << " base:has_count ?count" << i-0 << "\t" << "." << endl;
	}
	fout_type <<"}";
	fout_type.close();
/*
PREFIX:
PREFIX:

select ?type0 ?count0 ?type1 ?count1 ?type2 ?count2 where
{
	base:Barack_Obama rdf:type ?type0
	base:Xi_Jinping rdf:type ?type0
	?type0 base:has_count ?count0

	base:United_States rdf:type ?type1
	base:China rdf:type ?type1
	?type1 base:has_count ?count1

	base:Michelle_Obama rdf:type ?type2
	base:Peng_Liyuan rdf:type ?type2
	?type2 base:has_count ?type2
}
*/

/*
PREFIX:
PREFIX:

select ?p0 ?o0 ?sub0 ?pred0 where
{
	base:Barack_Obama ?p0 ?o0
	base:Xi_Jinping rdf:type ?type0
	?type0 base:has_count ?count0

	base:United_States rdf:type ?type1
	base:China rdf:type ?type1
	?type1 base:has_count ?count1

	base:Michelle_Obama rdf:type ?type2
	base:Peng_Liyuan rdf:type ?type2
	?type2 base:has_count ?type2
}
*/

	return 0;
}