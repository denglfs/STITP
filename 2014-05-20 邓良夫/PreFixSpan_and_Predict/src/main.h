#include "prefixSpan.h"
#include "Predict.h"
#include <time.h>

const char* sequenceFileName = "sequence.txt";
const char* pattnerFileName = "pattner.txt";
const char* sequenceForTestFileName = "testSequence.txt";
const char* expectNumFileName = "expectNum.txt";
void creat_sequence(int lines,int max_lineLen,int maxNum);//产生测试序列;
void creat_real_sequence();//生产预测序列和预测结果
void read_sequence(vector<vector<int>> & sequence,const char * filename);//读取序列到sequence 中去
void read_real_sequence(vector<vector<int>> & real_sequence,vector<vector<int>> &realNum);//读取预测序列 和 预测结果
bool IsInExpectSequence(int x,vector<int> &rhs);
void predict_test();
void format();
void format1();