#include "prefixSpan.h"
#include "Predict.h"
#include <time.h>

const char* sequenceFileName = "sequence.txt";
const char* pattnerFileName = "pattner.txt";
const char* sequenceForTestFileName = "testSequence.txt";
const char* expectNumFileName = "expectNum.txt";
void creat_sequence(int lines,int max_lineLen,int maxNum);//������������;
void creat_real_sequence();//����Ԥ�����к�Ԥ����
void read_sequence(vector<vector<int>> & sequence,const char * filename);//��ȡ���е�sequence ��ȥ
void read_real_sequence(vector<vector<int>> & real_sequence,vector<vector<int>> &realNum);//��ȡԤ������ �� Ԥ����
bool IsInExpectSequence(int x,vector<int> &rhs);
void predict_test();
void format();
void format1();