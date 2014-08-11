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
	double minConfidenceSupport;//��С���Ŷ�
	vector<Rules> rls;//�ƶ�����
	vector<pattner> ptr;//����ģʽ
	int PostFixNum(vector<int> lhs,vector<int> rhs);//����lhs��rhs����ͬ��׺����
	bool IsPreRules(vector<int> lhs,vector<int> rhs);//�ж�lhs�Ƿ�Ϊrhs��ǰ׺
	void ReadPattnerFromFile(const char * filename);//���ļ��ж�ȡģʽ
	
public:
	void makeRules(const char * PatternFileName);//�����ƶ�����
	void printRules();
	int FindMax(vector<int>& real);//Ԥ����
	predict(double minCs=0.6);
};
#endif
