#ifndef _STAYPOINT_H
#define _STAYPOINT_H

#include <vector>

using namespace std;
#include <iostream>
#include "global.h"

const double d = 2; //��·ָ��
const double t = 120; //ͣ����Сʱ��
const double l = 140; //�˶���̾���


class smallSP
{
public:
	int type;
	int pointNum;/////==x
	double duration;//ʱ��////===
	double length;//����////===x
	double pathLength;
	double volocity;//ƽ���ٶ�///===///x
	double detour; //��·ָ��
	vector<GPSPoint> list;
	bool getInfo()
	{
		if (list.size() <1)
			return false;
		duration =(double)(list.end()-1)->T - list.begin()->T;
		return true;
	}
	bool getInfox()
	{
		if (list.size() <1)
			return false;
		pointNum = list.size();	
		length =  GetPointsDistance(*list.begin(),*(list.end()-1));
		volocity = length / duration;
		pathLength = 0;
		vector<GPSPoint>::iterator it = list.begin();
		for(unsigned int i=0;i<list.size()-1;i++)
		{
			pathLength += (GetPointsDistance(*(it),*(it+1)));
			it ++;
			
		}
		detour = pathLength / length;
		return true;
	}
	smallSP()
	{
		type = -1;
		pointNum = -1;
		duration = -1;
		length =-1;
		volocity = -1;
		pathLength = -1;
		detour = -1;

	}
	double getDuration()
	{
		return  (double)((list.end()-1)->T - list.begin()->T);
	}
	void combineTwoSmallSP(smallSP& rhs)
	{
		list.insert(list.end(),rhs.list.begin(),rhs.list.end());
		this ->getInfo();

	}
	bool isSP(double timelimit,double detourlimit)
	{
		if ( duration >= timelimit && detour >= detourlimit)
			return true;
		return false;

	}
	bool isMP(double lengthlimit,double detourlimit)
	{
		//if(detour < detourlimit && length > lengthlimit
		this ->getInfox();
		if(length > lengthlimit)
			return true;
		return false;
	}
};


void GPSP2SP(vector<GPSPoint>& gpspoint, double timelimit, double vthreshold , vector<StopPoint>& out);
void ProcessNext(vector<GPSPoint>& gpspoint, bool* isnext, double vthreshold);
void MakeSP(vector<smallSP>& vecsp,vector<StopPoint>& out);
void smallCombine(vector<smallSP> &smallSPList, double moveTimelimit,double stopTimelimit);
void paragraphing(vector<GPSPoint>& gpspoint, vector<smallSP>& smallSPList,bool* isnext);//ͣ����ĳ��η�Ƭ
void writeStopPointTofile(vector<StopPoint>& out);
void writeGpsPointTofile(vector<GPSPoint>& gpspoint);
#endif