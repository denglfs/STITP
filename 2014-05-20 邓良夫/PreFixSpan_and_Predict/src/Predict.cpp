#include "Predict.h"
predict::predict(double minCs)
{
	this->minConfidenceSupport = minCs;
}
void predict::printRules()
{
	ofstream outf("rules");
	
	for(vector<Rules>::iterator itrls = rls.begin();itrls !=rls.end();itrls++)
	{
		outf<<itrls->support<<" :";
		for(vector<int>::iterator itpre = itrls->pre.begin(); itpre != itrls->pre.end();itpre++)
			outf<<*itpre<<" ";
		outf<<" -> ";
		for(vector<int>::iterator itpost = itrls->post.begin();itpost != itrls->post.end();itpost++)
			outf<<*itpost<<" ";
		outf<<endl;
	}
	outf.close();
}
int predict::PostFixNum(vector<int> lhs,vector<int> rhs)
{
	vector<int>::reverse_iterator itlhs = lhs.rbegin();
	vector<int>::reverse_iterator itrhs = rhs.rbegin();
	int ret = 0;
	for(;itlhs != lhs.rend() && itrhs != rhs.rend();itrhs++,itlhs++)
	{
		if (*itlhs != *itrhs )
			break;
		ret ++ ;
	}
	return ret;
}
int predict::FindMax(vector<int> &real)
{
	
	double max = -1;
	int ret = -1;
	for(int i=0;i<rls.size();i++)
	{
		int len = PostFixNum(real,rls[i].pre);
		//double tmpMax = len * rls[i].support;
		double tmpMax = (1.0 * len) / real.size() + rls[i].support;
		if (len ==0)
			tmpMax =0;
		if (max < tmpMax)
		{	
			ret = rls[i].post[0];
			max = tmpMax;
		//	cout<<"len = "<<len<<endl<<"max= "<<max<<endl;
		}

	}
	//cout<<"max="<<max<<endl;
	
	return max==0?-1:ret;
}
bool predict::IsPreRules(vector<int> lhs,vector<int> rhs)
{
	if (lhs.size() > rhs.size()) return false;
	for(int i = 0;i<lhs.size();i++)
	{
		if(rhs[i] != lhs[i] )
			return false;
	}
}
void predict::makeRules(const char * PattnerFileName)
{
	ReadPattnerFromFile(PattnerFileName);
	for(int i=0;i<ptr.size() -1 ;i++)
	{
		for(int j = i+1;j<ptr.size();j++)
		{
			if(IsPreRules(ptr[i].patr,ptr[j].patr))
			{
				double sup=double(ptr[j].support)/double(ptr[i].support);
				if( sup > minConfidenceSupport)
				{
					Rules tmp;
					vector<int> x;
					for( int k = ptr[i].patr.size();k<ptr[j].patr.size();k++)
						x.push_back(ptr[j].patr[k]);

					tmp.pre=ptr[i].patr;
					tmp.post=x;
					tmp.support = sup;
					rls.push_back(tmp);
				}
			}
		}
	}
}
void predict::ReadPattnerFromFile(const char *filename)
{
	ifstream inf(filename);
	string str;
	getline(inf,str);
	while(getline(inf,str))
	{
		pattner tmp;
		istringstream strinf(str);
		strinf>>tmp.support;
		int x;
		while(strinf >> x)
		{
			tmp.patr.push_back(x);
		}
		ptr.push_back(tmp);
	}
	inf.close();
}