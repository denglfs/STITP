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
  vector < vector<int> >    transaction;//保存所有的字符
  vector <vector <int> >        record;//记录每行字符串大小(黑色屏幕中出现的那些)
  vector < pair <int, unsigned int> > pattern;//pattern---保存识别出的字符序列
  double minsup;
  unsigned int minSupLen;
  unsigned int minpat;
  unsigned int maxpat;
  bool all;
  ostream *os;
  void report (vector <pair <unsigned int, int> > &projected);//report---输出识别出的字符序列
  void project (vector <pair <unsigned int, int> > &projected);//构造序列，并 保存在pattern中
  istream& read (istream &is);
  
  void clear ();
public:
	PrefixSpan (double _minsup = 2,unsigned int _minpat = 1,unsigned int _maxpat = 0xffffffff,bool _all = true);
	~PrefixSpan();
    ostream& run (const char * sequenceFileName,const char *patternFileName);//参数1:输入文件 参数2：输出文件
    
};

#endif