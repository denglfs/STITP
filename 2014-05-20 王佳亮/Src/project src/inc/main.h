#ifndef _MAIN_H
#define _MAIN_H

#include "global.h"

int main();

//完整的论文步骤
void test_paper();
void test_paper2();
void test_paper3();

//////////

void tempfunc2(vector<StopPointex>& allspex); //输出簇序列,每个用户一个文件,每个PLT一行
bool SORTOPTICSCMPARVT(const StopPointex& a, const StopPointex& b); //sort比较函数 按arvt升序

void OutputAllSPEX(vector<StopPointex>& allspex); //把所有spex属性存文件
void OutputAllSPEXLatLng(vector<StopPointex>& allspex); //把所有spex经纬存文件

void ReadSPEXLatLng(vector<StopPointex>& spex); //从文件读取spex经纬
void ReadSPEX(vector<StopPointex>& spex); //从文件读取spex所有属性

void CalStopPoint(SysOptions& sysopt, Logger& logger, vector<StopPoint>& allsp); //生成停留点

#endif