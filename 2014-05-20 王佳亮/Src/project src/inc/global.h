#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <list>
#include <vector>
#include <algorithm>

using namespace std;

#include <time.h>
#include <windows.h>

//��ȫ�ͷŶ�̬�ռ�
#define SAFE_DELETE(p)\
{\
	delete (p);\
	(p) = NULL;\
}

#define SAFE_DELETE_ARRAY(p)\
{\
	delete [](p);\
	(p) = NULL;\
}

#define FLOAT_EQUAL 1e-5 //������0
#define PI 3.1415926
#define EARTH_RADIUS 6378.137 //����뾶

//ϵͳ����
struct SysOptions
{
	int usernum;
	double timelimit;
	double vthreshold;
	double eps;
	unsigned int minpts;
	int uid;
	double p;
};

//��ʱ��
class TimeCounter
{
protected:
	DWORD m_dwStart;
	DWORD m_dwEnd;

public:
	TimeCounter();

	void Start();
	void End();
	DWORD GetCount();
};

//log��¼
class Logger
{
protected:
	string m_strFile;

public:
	Logger();

	bool Log(char* str);
};

//��
struct Point
{
	double Lat; //γ��
	double Lng; //����
	double H; //����

	long UserID;
	long FileID; //����PLT�ļ���ʶ

	Point();
	Point(double lat, double lng, double h, unsigned long user);

	bool operator ==(const Point& p) const;
};

//GPS��
struct GPSPoint: public Point
{
	time_t T; //ʱ��

	GPSPoint();
	GPSPoint(double lat, double lng, double h, time_t& t, unsigned long user);
};

//ͣ����
struct StopPoint: public Point
{
	time_t ArvT; //����ʱ��
	time_t LevT; //�뿪ʱ��

	StopPoint();
	StopPoint(double lat, double lng, double h, time_t& arvt, time_t& levt, unsigned long user);

	bool operator ==(const StopPoint& p) const;
};

//���ر��,״̬���,�ɴ����ͺ��ľ���(OPTICS)��ͣ����
#define STATE_SPEX_UNVISITED 0
#define STATE_SPEX_VISITED 1
#define STATE_SPEX_NOISE 2

#define REACHDIS_SPEX_UNDEFINED -1
#define COREDIS_SPEX_UNDEFINED -1
#define ID_SPEX_UNDEFINED -1

struct StopPointex: public StopPoint
{
	long long CID;
	int State;

	double ReachDis; //�ɴ����
	double CoreDis; //���ľ���

	double OPTICSOF; //opticsof

	long long ID;

	StopPointex();

	bool operator ==(const StopPointex& p) const;

	void SetSP(StopPoint& sp);
};

//��
struct Cluster
{
	long long ID;

	time_t ArvT;
	time_t LevT;
};

//�����
struct ShareCluster
{
	long long ClusterID1;
	long long ClusterID2;

	ShareCluster();
	ShareCluster(long long c1, long long c2);
};

//ͼ�ı߽ڵ�
template <typename TWeightType>
struct EdgeNode
{
	int iAdjoinVertex; //�ڽӶ���
	TWeightType Weight; //���������ڽӶ���֮��ߵ�Ȩ

	EdgeNode* pNext;

	EdgeNode();
};

template <typename TWeightType>
EdgeNode<TWeightType>::EdgeNode()
{
	pNext = NULL;
}

//���ܺ���
double DegToRad(double degree); //���Ƕ�ת��Ϊ����
double RadToDeg(double radian); //������ת��Ϊ�Ƕ�
double GetPointsDistance(Point& p1, Point& p2); //������POINT֮�����(��λΪ��)
bool IsEqualTM(const tm& t1, const tm& t2); //�ж�2��tm�ṹ�Ƿ����

//�ж�2��list�Ƿ���ͬ
template <typename T>
bool IsListsSame(list<T>& list1, list<T>& list2)
{
	for(list<T>::iterator it = list1.begin(); it != list1.end(); ++it)
	{
		for(list<T>::iterator it2 = list2.begin(); it2 != list2.end(); ++it2)
		{
			if( *(it) != *(it2) )
			{
				return false;
			}
		}
	}

	return true;
}

//�ϲ�2��vector,v2�ӵ�v1����
template <typename T>
void CombineVector(vector<T>& v1, vector<T>& v2)
{
	for(vector<T>::iterator it = v2.begin(); it != v2.end(); ++it)
	{
		v1.push_back(*it);
	}
}

//�ϲ�2��list,l2�ӵ�l1����
template <typename T>
void CombineList(list<T>& l1, list<T>& l2)
{
	for(list<T>::iterator it = l2.begin(); it != l2.end(); ++it)
	{
		l1.push_back(*it);
	}
}

//�ϲ�2��list,l2�ӵ�l1����,l1������Ԫ�ز��ظ�����
template <typename T>
void CombineListNoRepeat(list<T>& l1, list<T>& l2)
{
	for(list<T>::iterator it = l2.begin(); it != l2.end(); ++it)
	{
		list<T>::iterator findpos = find(l1.begin(), l1.end(), *it);
		if( findpos == l1.end() )
		{
			l1.push_back(*it);
		}
	}
}

//��ȡ2άvector����һ��Ԫ����Χ��Ԫ��
template <typename T>
void GetAroundFrom2DVector(int r, int c, vector<vector<T>>& vec, vector<T>& out)
{
	bool rp = r + 1 > vec.size() - 1 ? false : true; //��������
	bool rm = r - 1 < 0 ? false : true; //��������
	bool cp = c + 1 > vec.front().size() - 1 ? false : true; //��������
	bool cm = c - 1 < 0 ? false : true; //��������

	if( rm )
	{
		//�Ϸ�
		out.push_back(vec[r - 1][c]);
		if( cm )
		{
			//���Ͻ�
			out.push_back(vec[r - 1][c - 1]);
		}
		if( cp )
		{
			//���Ͻ�
			out.push_back(vec[r - 1][c + 1]);
		}
	}
	if( rp )
	{
		//�·�
		out.push_back(vec[r + 1][c]);
		if( cm )
		{
			//���½�
			out.push_back(vec[r + 1][c - 1]);
		}
		if( cp )
		{
			//���½�
			out.push_back(vec[r + 1][c + 1]);
		}
	}
	if( cm )
	{
		//��
		out.push_back(vec[r][c - 1]);
	}
	if( cp )
	{
		//�ҷ�
		out.push_back(vec[r][c + 1]);
	}
}

void SP2SPEX(vector<StopPoint>& sp, vector<StopPointex>& spex); //ͣ����ת����ǵ�ͣ����

double CalAverPointDis(double a, int n); //ƽ�������ƹ�ʽ

#endif