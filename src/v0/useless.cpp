#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<utility>
#include<map>
#include<vector>
#include<algorithm>

using namespace std;

#define SET 10
#define ENTITY_NUM 10
#define LENGTH 2
#define PATH_NUM 2000
#define OUT_DEGREE 100 // 每个entity的重要出度的max
#define IN_DEGREE 100  // 每个entity的重要入度的max

class Path1 // 长度＝1
{
  public:
  	pair <string,string>a;
  	//int set_num; // 属于第几组，总共有cnt组
  	int different_set;
  	double dominence;
  	int appear_times;
  	int out; // 相对于example entity是出边 一个这样的pair不可能既是out，又是in
  	int in;  // 相对于example entity是入边
  	bool operator < (const Path1& other)  const 
    {
        return (dominence < other.dominence);
    }
};

//<Element_electronics>  	rdf:type       	<wikicat_Electronics_companies_of_the_United_States>
//<Baden,_Morbihan>      	rdf:type       	<wikicat_Communes_of_Morbihan>

int main(char* argv[])
{
	ifstream fin(argv[1]); // 用户输入，txt
	ifstream fin2(argv[2]); // fact
	ifstream fin3(argv[3]); // type
	ofstream fout(argv[4]); // output SPARQL伪代码的形式
	ofstream fout("string_to_type.txt");
	map<string,vector<string> >str_to_types;
	map<string,int>str_to_set; // 这个entity在哪一组里, 从0开始编号
	map<string,vector<pair<string,string> > >sub_to_pred_and_obj;
	map<string,vector<pair<string,string> > >obj_to_sub_and_pred;

	vector<string>type[ENTITY_NUM];
	//vector<string>type2[ENTITY_NUM];


	//variables for fin
	int entity_num;     // 输入的一组有entity_num个entities
	string str[SET][ENTITY_NUM]; // 输入的所有实例string
	int cnt = 0;  // 有多少组实例

	//variables for fin2
	string sub_fact, pred_fact, obj_fact;

	//variables for fin3
	string sub_type, pred_type, obj_type;

	while (fin >> entity_num)
	{
		
		for (int i = 0; i < entity_num; i++)
		{
			fin >> str[cnt][i];
			str_to_set[str[cnt][i]] = cnt;
		}
		cnt++;
	}
   
    //共有cnt组，每组有entity_num个entities


    while(fin3 >> sub_type >> pred_type >> obj_type)
    {
    	str_to_types[sub_type].push_back(obj_type);
    }


    /* naive type infer(data:yago simple types)
    deal with types: first add type to sparql
    适用于一组example有多个entity的
    暂时还没有考虑权重
    */
    // 取每个entity共同的type, 填充vector<string>type[i]
    for (int i = 0; i < entity_num; i++)
    {
    	vector<string>type[i].push_back(str_to_types[str[0][i]]); // may map to several different types
    	for (int j = 1; j < cnt; j++)
    	{
    		vector<string>type2[ENTITY_NUM];
    		vector<string>::iterator iter;
    		vector<string>temp_type = str_to_types[str[j][i]];
    		
    		for (iter = temp_type[i].begin(); iter != temp_type[i].end(); iter++)
    		{
    			if (find(type[i].begin(), type[i].end(), *iter) != type[i].end())
    			{
    				type2[i].push_back(*iter);
    			}
    		}
    		type[i].swap(type2[i]); // 这个方法有点厉害
    	}
    }

    for (int i = 0; i < entity; i++)  // every entity is a variable
    {
    	 int size_ = type[i].size();
    	 for (int j = 0; j < size_; j++)
    	 {
    	 	// 同样的数字到时候替换成相同的variable
    	 	fout << i << "\t" << "rdf:type" << "\t" << type[i][j] << endl;
    	 }
    }

    /* we are then going to consider a similarity model 
    based on the neighbourhood patterns of the example entities and statistical record of the original dataset*/
    
    //一些不重要的边事先预处理的时候删除（可以事先计算出所有边的出现频率，过低的删除之类的） 

    /*思路：分两种情况讨论：长度为1的paths和长度大于1的paths
    暂时先只考虑输入一组example只有一个entity的情况，如果有多类entity，对每种entity竖着都来一下这个过程就行了
    对于长度等于1的paths来讲：相当于有一堆（predicate, object）/ (subject, predicate)的pair，要找出其中domiment的 （用类似文档里tf*idf的想法）
    对于长度大于1的paths来讲：每条路径建模成一个特征向量，衡量路径两两之间的相似度［similarity matrix］，
                           然后取和所有其他路径相似度最高的k条路径添加到sparql中去
    */
    // path=1: domimence的衡量：#pair * #entity containing pair / #entity(保证出现得多且分布均匀)
    // To be done: 还有一些静态信息没有加
	while (fin2 >> sub_fact >> pred_fact >> obj_fact)
    {
    	sub_to_pred_and_obj[sub_fact].push_back(std::make_pair(pred_fact, obj_fact));
    	obj_to_sub_and_pred[obj_fact].push_back(std::make_pair(sub_fact, pred_fact));
    }
    for (int i = 0; i < entity; i++) // 对每一个竖着的i，都要这么来一下
    {
    	Path1 path1[SET*OUT_DEGREE];  // 对于每一组竖的，都要重新生成
    	for (int j = 0; j < SET*OUT_DEGREE, j++)
    	{
    		path1[j].appear_times = 0;
    		path1[j].different_set = 0;
    	}
    	//int temp = 0;
    	int index_ = 0;
    	for (int j = 0; j < cnt; j++)
    	{
    		string s_now = str[j][i];
    		vector<pair<string,string> >temp1 = sub_to_pred_and_obj[str[j][i]];
    		vector<pair<string,string> >temp2 = obj_to_sub_and_pred[str[j][i]];
    		if (temp1.size() != 0)
    		{
    			path1[index_].a = temp1[0];
    			path1[index_].dominence = 0;
    			path1[index_].appear_times++;
    			path1[index_].different_set++;
    			path1[index_].out = 1;
    			path1[index_].in = 0;
    		}
    		for(int k = 1; k < temp1.size(); k++)
    		{
    			int flag = 1;
    			for (int v = 0; v < index_; v++)
    				if (((path1[v].a) -> first == temp1[k]->first) && ((path1[v].a) -> second == temp1[k]->second))
    				{
    					path1[v].appear_times++;
    					flag = 0;
    					break;
    				}
    			if (flag) // 没有找到一样的
    			{
    				index_++;
					path1[index_].a = temp1[k];
    				//path1[k].set_num = j;
    				path1[index_].dominence = 0;
    				path1[index_].appear_times++;
    				path1[index_].different_set++;
    				path1[index_].out = 1;
    				path1[index_].in = 0;
    		    }
    		}
    		for (int k = 0; k < temp2.size(); k++)
    		{
    			int flag = 1;
    			for (int v = 0; v < index_; v++)
    				if (((path1[v].a) -> first == temp2[k]->first) && ((path1[v].a) -> second == temp2[k]->second))
    				{
    					path1[v].appear_times++;
    					flag = 0;
    					break;
    				}
    			if (flag) // 没有找到一样的
    			{
    				index_++;
    				path1[index_].a = temp2[k];
    				//path1[k + temp1.size()].set_num = j;
    				path1[index_].dominence = 0;
    				path1[index_].appear_times++:
    				path1[index_].different_set++;
    				path1[index_].out = 0;
    				path1[index_].in = 1;
    			}
    		}
    	}
    	
    	int total_pair = index_ + 1;
    	for (int j = 0; j < total_pair; j++)
    	{
    		path1[j].dominence = path1[j].appear_times * path1[j].different_set;
    	}

    	sort(path1, path1 + total_pair);

    	for (int j = 0; j < 2; j++) // 取前2个
    	{
    		if (path1[j].out == 1)
    		{
    			fout << i << "\t" << (path1[j].a)->first << "\t" << (path1[j].a)-> second << endl;
    		}
    		else
    		{
    			fout << (path1[j].a)->first << "\t" << (path1[j].a)->second<< "\t" << i << endl;
    		}
    	}
    	
    }


    /*[Don't Breathe, Stephen Lang], [Captain Fantastic, Viggo Mortensen]*/
    /*如果要考虑横向之间的关系的话，以下所有都是在每组实例个数大于1的时候需要处理的。
    以上偏重挖掘共有属性，这里偏重挖掘相同的关系
    如果只有一种entity，用上面的过程就够了。

    ＊＊＊＊＊第二部分：把每组多于一个entity的重要关系挖掘出来，加入sparql
    ** To be continue
    */
    /*if (entity_num > 1)  
    {
       
    }
   */

	return 0;
}










