#ifndef _MAIN_H
#define _MAIN_H

#include "global.h"

int main();

//���������Ĳ���
void test_paper();
void test_paper2();
void test_paper3();

//////////

void tempfunc2(vector<StopPointex>& allspex); //���������,ÿ���û�һ���ļ�,ÿ��PLTһ��
bool SORTOPTICSCMPARVT(const StopPointex& a, const StopPointex& b); //sort�ȽϺ��� ��arvt����

void OutputAllSPEX(vector<StopPointex>& allspex); //������spex���Դ��ļ�
void OutputAllSPEXLatLng(vector<StopPointex>& allspex); //������spex��γ���ļ�

void ReadSPEXLatLng(vector<StopPointex>& spex); //���ļ���ȡspex��γ
void ReadSPEX(vector<StopPointex>& spex); //���ļ���ȡspex��������

void CalStopPoint(SysOptions& sysopt, Logger& logger, vector<StopPoint>& allsp); //����ͣ����

#endif