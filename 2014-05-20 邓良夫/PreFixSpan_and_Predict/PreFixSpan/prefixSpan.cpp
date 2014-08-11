#include "prefixSpan.h"

void PrefixSpan::report (vector <pair <unsigned int, int> > &projected)
{
    if (minpat > pattern.size()) return;//����Ҫ������С����
	
    *os <<projected.size()<<"\t";
	for (unsigned int i = 0; i < pattern.size(); i++)
        *os <<(pattern[i].first)<<' ';
	*os<<endl;
}

void PrefixSpan::project (vector <pair <unsigned int, int> > &projected)
{
    if (all)
        report(projected);//����ҵ�������

    map <int, vector <pair <unsigned int, int> > > counter;//ֵ:{�кţ�����λ��;�кţ�����λ�� }
   //��ÿ�н��д������������ַ����ĵ�һ�γ��ֵ��Ӵ��������е�λ��
    for (unsigned int i = 0; i < projected.size(); i++)
    {//��ÿ�д���
          int pos = projected[i].second;//root ֵΪ-1
          unsigned int id  = projected[i].first;//�к�
          unsigned int size = transaction[id].size();//�����г���
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
          //��ÿһ�г��ֵ��Ӵ��������������� ��ֵ:{�кţ�����λ��;�кţ�����λ�ã�... }�ķ�ʽ����  
		  for (map <int, int>::iterator k = tmp.begin(); k != tmp.end(); ++k)
                counter[k->first].push_back( make_pair(id, k->second));
    }
//����СƵ��С��ֵҪɾ��
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
    //���ڱ����������Ӵ�����֧�ֶȴ���2��

    for (map <int, vector <pair <unsigned int, int> > >::iterator l = counter.begin ();  l != counter.end(); ++l)
    {
      if (pattern.size () < maxpat)
      {
         ///���Ӵ����������ŵ�pattern���棬
         pattern.push_back (make_pair (l->first, l->second.size()));
         ///��{�кţ�����λ��;�кţ�����λ�ã�... }�ٴδ���project������

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
           while (istrs >> item) tmp.push_back (item);//ÿ�е����ݱ�����vector<string>��
          // istrstream iistrs ((char *)line.c_str());
           //while(iistrs >> cc)
            //{temp.push_back(cc.size());cout<<cc.size()<<' ';}
           transaction.push_back (tmp);//transaction �൱��һ����ά���鱣������������
           //record.push_back(temp);
        }
        //cout<<record.size()<<record[0][0]<<record[0][1];
		minSupLen = minsup * transaction.size();
		minSupLen = minSupLen > 2? minSupLen :2;
		cout<<"��С֧������"<<minSupLen<<endl;
        return is;
}

ostream& PrefixSpan::run (const char * sequenceFile,const char *pattern)
{
		ifstream inf(sequenceFile);
		read(inf);
		ofstream outf(pattern);
        os = &outf;
        *os <<"total lines:"<<transaction.size() << endl;//����
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