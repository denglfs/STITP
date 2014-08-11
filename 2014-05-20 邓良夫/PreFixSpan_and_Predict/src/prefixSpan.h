#ifndef _PREFIXSPAN_H
#define _PREFIXSPAN_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <strstream>
#include <io.h>
#include <process.h>
#include <stdlib.h>
#include <fstream>
#include <utility>
using namespace std;

class PrefixSpan
{
private:
  vector < vector<int> >    transaction;//�������е��ַ�
  vector <vector <int> >        record;//��¼ÿ���ַ�����С(��ɫ��Ļ�г��ֵ���Щ)
  vector < pair <int, unsigned int> > pattern;//pattern---����ʶ������ַ�����
  double minsup;
  unsigned int minSupLen;
  unsigned int minpat;
  unsigned int maxpat;
  bool all;
  ostream *os;
  void report (vector <pair <unsigned int, int> > &projected);//report---���ʶ������ַ�����
  void project (vector <pair <unsigned int, int> > &projected);//�������У��� ������pattern��
  istream& read (istream &is);
  
  void clear ();
public:
	PrefixSpan (double _minsup = 2,unsigned int _minpat = 1,unsigned int _maxpat = 0xffffffff,bool _all = true);
	~PrefixSpan();
    ostream& run (const char * sequenceFileName,const char *patternFileName);//����1:�����ļ� ����2������ļ�
    
};

#endif