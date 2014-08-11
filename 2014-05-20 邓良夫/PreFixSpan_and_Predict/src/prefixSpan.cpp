#include "prefixSpan.h"

void PrefixSpan::report (vector <pair <unsigned int, int> > &projected)
{
    if (minpat > pattern.size()) return;//序列要大于最小长度
	
    *os <<projected.size()<<"\t";
	for (unsigned int i = 0; i < pattern.size(); i++)
        *os <<(pattern[i].first)<<' ';
	*os<<endl;
}

void PrefixSpan::project (vector <pair <unsigned int, int> > &projected)
{
    if (all)
        report(projected);//输出找到的序列

    map <int, vector <pair <unsigned int, int> > > counter;//值:{行号，行中位置;行号，行中位置 }
   //对每行进行处理，保存所有字符串的第一次出现的子串及在行中的位置
    for (unsigned int i = 0; i < projected.size(); i++)
    {//对每行处理
          int pos = projected[i].second;//root 值为-1
          unsigned int id  = projected[i].first;//行号
          unsigned int size = transaction[id].size();//所在行长度
          map <int, int> tmp;
        //  int low=0,high=0,len,start;
		
          for(unsigned int j = pos + 1; j < size; j++)
          {
            int pr_string=transaction[id][j];
            /*string mine;
            for(len=pr_string.size();len>=1;len--)
                for(start=0;start+len<=pr_string.size();start++)
                {
                    for(int t=start;t<start+len;t++)
                    {
                        mine+=pr_string[t];
                    }
                    if (tmp.find (mine) == tmp.end())
                        tmp[mine] = j;
                    mine="";
                }*/
			if (tmp.find(pr_string) == tmp.end())
				tmp[pr_string] = j;
			//break;
          }
          //把每一行出现的子串汇总起来，按照 串值:{行号，行中位置;行号，行中位置，... }的方式保存  
		  for (map <int, int>::iterator k = tmp.begin(); k != tmp.end(); ++k)
                counter[k->first].push_back( make_pair(id, k->second));
    }
//比最小频率小的值要删掉
    for (map <int, vector <pair <unsigned int, int> > >::iterator l = counter.begin ();l != counter.end (); )
    {
		if (l->second.size() < minSupLen)
        {
            map <int, vector <pair <unsigned int, int> > >::iterator tmp = l;
            tmp = l;
            ++tmp;
            counter.erase (l);
            l = tmp;
        }
        else
        {
            ++l;
        }
    }
    //现在保留下来的子串都是支持度大于2的

    for (map <int, vector <pair <unsigned int, int> > >::iterator l = counter.begin ();  l != counter.end(); ++l)
    {
      if (pattern.size () < maxpat)
      {
         ///把子串存起来，放到pattern里面，
         pattern.push_back (make_pair (l->first, l->second.size()));
         ///把{行号，行中位置;行号，行中位置，... }再次传入project函数中

		 project (l->second);
         pattern.pop_back();
      }
    }
}
istream& PrefixSpan::read (istream &is)
{
        string line;
        vector <int> tmp;
        int item;
        string cc;
        //record;
        //vector<int>temp;
        while (getline (is, line))
        {
           tmp.clear ();
           //temp.clear();
           istrstream istrs ((char *)line.c_str());
           while (istrs >> item) tmp.push_back (item);//每行的数据保存在vector<string>里
          // istrstream iistrs ((char *)line.c_str());
           //while(iistrs >> cc)
            //{temp.push_back(cc.size());cout<<cc.size()<<' ';}
           transaction.push_back (tmp);//transaction 相当于一个二维数组保存了所有数据
           //record.push_back(temp);
        }
        //cout<<record.size()<<record[0][0]<<record[0][1];
		minSupLen = minsup * transaction.size();
		minSupLen = minSupLen > 2? minSupLen :2;
		cout<<"最小支持数："<<minSupLen<<endl;
        return is;
}

ostream& PrefixSpan::run (const char * sequenceFile,const char *pattern)
{
		ifstream inf(sequenceFile);
		read(inf);
		ofstream outf(pattern);
        os = &outf;
        *os <<"total lines:"<<transaction.size() << endl;//行数
        vector <pair <unsigned int, int> > root;
        for (unsigned int i = 0; i < transaction.size(); i++)
            root.push_back (make_pair (i, -1));
        project (root);
        return *os;
}

void PrefixSpan::clear ()
{
        transaction.clear ();
        pattern.clear ();
		record.clear();
}

PrefixSpan::PrefixSpan(double _minsup,unsigned int _minpat,unsigned int _maxpat,bool _all)
{
	minsup = _minsup;
    minpat = _minpat;
	maxpat = _maxpat;
	all = _all;
}
PrefixSpan::~PrefixSpan()
{
	clear();
}