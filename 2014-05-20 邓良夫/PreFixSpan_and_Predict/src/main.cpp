
#include "main.h"

void creat_sequence(int lines,int max_lineLen,int maxNum)//产生测试序列
{
	ofstream outf(sequenceFileName);
	srand(time(NULL));
	for(int i=0;i<lines;i++)
	{
		
		int linelen = rand()%max_lineLen+3;
		for(int j=0;j<linelen;j++)
		{
			int tmpNum = rand()%maxNum;
			outf<<tmpNum<<" ";
		}
		outf<<endl;
	}
	outf.close();
}

void read_sequence(vector<vector<int>> & sequence,const char * filename)
{//读取序列到sequence 中去
	ifstream inf(filename);
	string str;
	while (getline(inf,str))
	{
		vector<int> x;
		istringstream ifstr(str);
		int tmp;
		while(ifstr >> tmp)
			x.push_back(tmp);
		sequence.push_back(x);
	}
	inf.close();
}
void creat_real_sequence()
{
	
	ofstream outf(sequenceForTestFileName);
	ofstream outf1(expectNumFileName);
	vector<vector<int>>  sequence;
	read_sequence(sequence,sequenceFileName); //历史读取序列
	srand(time(NULL));
	for(int i= 0 ; i<sequence.size();i++)
	{
		int index = rand() % (sequence[i].size()-1);
		for(vector<int>::iterator it = sequence[i].begin();it != sequence[i].begin()+index+1;it++)
		{
			outf<<*it<<" ";
		}
		outf<<endl;
		for(vector<int>::iterator it = sequence[i].begin()+index+1;it!=sequence[i].end();it++)
			outf1<<*it<<" ";
		outf1<<endl;
	}
	outf1<<endl;
	outf1.close();
	outf.close();

	
}
void read_real_sequence(vector<vector<int>> & real_sequence,vector<vector<int>> &realNum)
{
	//读取序列到real_sequence 中去
	ifstream inf(sequenceForTestFileName);
	string str;
	while (getline(inf,str))
	{
		vector<int> x;
		istringstream ifstr(str);
		int tmp;
		while(ifstr >> tmp)
			x.push_back(tmp);
		real_sequence.push_back(x);
	}
	inf.close();

	ifstream inf1(expectNumFileName);

	while (getline(inf1,str))
	{
		vector<int> x;
		istringstream ifstr(str);
		int tmp;
		while(ifstr >> tmp)
			x.push_back(tmp);
		realNum.push_back(x);
	}
	inf1.close();

	
}
void predict_test()
{
	double confidence =0;
	cout<<"confidence:";
	cin>>confidence;
	predict pre(confidence);
	pre.makeRules(pattnerFileName);
	//生成移动规则
	pre.printRules();
	vector<vector<int>> realSeq;
	vector<vector<int>> realPos;
	vector<int> predictPos;
	read_real_sequence(realSeq,realPos);
	
	for(int i= 0 ; i<realSeq.size();i++)
	{
		predictPos.push_back(pre.FindMax(realSeq[i]));

		cout<<predictPos[i]<<":   ";
		for(vector<int>::iterator it =realSeq[i].begin();it != realSeq[i].end();it++)
			cout<<*it<<" ";
		cout<<"->";
		for(vector<int>::iterator it =realPos[i].begin();it != realPos[i].end();it++)
			cout<<*it<<" ";
		cout<<endl;
	}
	int total = 0;
	int num_1=0;
	for(int i=0;i<predictPos.size();i++)
	{
		if(predictPos[i] == -1)
			num_1++;
		if( IsInExpectSequence(predictPos[i],realPos[i]) == true )
			++total;
	}
	cout<<"一共预测： "<<predictPos.size()<<" 个"<<endl;
	cout<<"有效预测： "<<predictPos.size() - num_1<<" 个"<<endl;
	cout<<"准确预测： "<<total<<" 个"<<endl;
	cout<<"准确度："<< total * 1.0 / (predictPos.size()-num_1)<<endl;;

}
bool IsInExpectSequence(int x,vector<int> &rhs)
{
	for (vector<int>::iterator it = rhs.begin();it != rhs.end();it++)
	{
		if ( x == *it)
			return true;
	}
	return false;
}
void format()
{
	vector<vector<int>> sequence;
	ofstream outf(sequenceFileName);
	read_sequence(sequence,"old_data.txt");
	for(int i=0;i<sequence.size();i++)
	{
		vector<int> tmp;
		int cur=sequence[i][0];
		tmp.push_back(cur);
		for(int j=0;j<sequence[i].size();j++)
		{
			if(cur != sequence[i][j] && sequence[i][j] != -1)
			{
				tmp.push_back(sequence[i][j]);
				cur = sequence[i][j];
			}
		}
		if (tmp.size() >= 2 )
		{
			for(vector<int>::iterator it = tmp.begin();it != tmp.end(); it++)
				outf<<*it<<" ";
			outf<<endl;
		}
	}
}
void format1()
{
	ifstream inf("old_data.txt");
	ofstream outf(sequenceFileName);
	string str;
	while (getline(inf,str))
	{
		char c;
		istringstream instr(str);
		instr>>c;
		int tmp;
		while(instr >> tmp)
			outf<<tmp<<" ";
		outf<<endl;
	}
}
int main (int argc, char **argv)
{
	 format();
	creat_real_sequence();
	double minsup = 0.6;
	  cout<<"input the minsup:";
	  cin>>minsup;

	  PrefixSpan prefixspan(minsup);
	  prefixspan.run  (sequenceFileName,pattnerFileName);
	  cout<<"模式产生完成"<<endl;
	  ///生产运动模式
	  while (1)
	  {
		  predict_test();
		  system("pause");
	  }
	  


	  return 0;
}
