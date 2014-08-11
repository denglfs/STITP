#ifndef _PREDICT_H
#define _PREDIcT_H

#include<vector>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

struct Rules
{
	vector<int> pre;
	vector<int> post;
	double support;
};
struct  pattner
{
	vector<int> patr;
	int support;
};
class predict
{
private:
	double minConfidenceSupport;//最小置信度
	vector<Rules> rls;//移动规则
	vector<pattner> ptr;//序列模式
	int PostFixNum(vector<int> lhs,vector<int> rhs);//返回lhs和rhs的相同后缀长度
	bool IsPreRules(vector<int> lhs,vector<int> rhs);//判断lhs是否为rhs的前缀
	void ReadPattnerFromFile(const char * filename);//从文件中读取模式
	
public:
	void makeRules(const char * PatternFileName);//生产移动规则
	void printRules();
	int FindMax(vector<int>& real);//预测结果
	predict(double minCs=0.6);
};
#endif
