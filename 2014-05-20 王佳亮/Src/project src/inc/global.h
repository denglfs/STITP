#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <list>
#include <vector>
#include <algorithm>

using namespace std;

#include <time.h>
#include <windows.h>

//安全释放动态空间
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

#define FLOAT_EQUAL 1e-5 //浮点判0
#define PI 3.1415926
#define EARTH_RADIUS 6378.137 //地球半径

//系统参数
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

//计时器
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

//log纪录
class Logger
{
protected:
	string m_strFile;

public:
	Logger();

	bool Log(char* str);
};

//点
struct Point
{
	double Lat; //纬度
	double Lng; //经度
	double H; //海拔

	long UserID;
	long FileID; //所属PLT文件标识

	Point();
	Point(double lat, double lng, double h, unsigned long user);

	bool operator ==(const Point& p) const;
};

//GPS点
struct GPSPoint: public Point
{
	time_t T; //时间

	GPSPoint();
	GPSPoint(double lat, double lng, double h, time_t& t, unsigned long user);
};

//停留点
struct StopPoint: public Point
{
	time_t ArvT; //到达时间
	time_t LevT; //离开时间

	StopPoint();
	StopPoint(double lat, double lng, double h, time_t& arvt, time_t& levt, unsigned long user);

	bool operator ==(const StopPoint& p) const;
};

//带簇标号,状态标记,可达距离和核心距离(OPTICS)的停留点
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

	double ReachDis; //可达距离
	double CoreDis; //核心距离

	double OPTICSOF; //opticsof

	long long ID;

	StopPointex();

	bool operator ==(const StopPointex& p) const;

	void SetSP(StopPoint& sp);
};

//簇
struct Cluster
{
	long long ID;

	time_t ArvT;
	time_t LevT;
};

//共享簇
struct ShareCluster
{
	long long ClusterID1;
	long long ClusterID2;

	ShareCluster();
	ShareCluster(long long c1, long long c2);
};

//图的边节点
template <typename TWeightType>
struct EdgeNode
{
	int iAdjoinVertex; //邻接顶点
	TWeightType Weight; //自身顶点与邻接顶点之间边的权

	EdgeNode* pNext;

	EdgeNode();
};

template <typename TWeightType>
EdgeNode<TWeightType>::EdgeNode()
{
	pNext = NULL;
}

//功能函数
double DegToRad(double degree); //将角度转换为弧度
double RadToDeg(double radian); //将弧度转换为角度
double GetPointsDistance(Point& p1, Point& p2); //计算两POINT之间距离(单位为米)
bool IsEqualTM(const tm& t1, const tm& t2); //判断2个tm结构是否相等

//判断2个list是否相同
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

//合并2个vector,v2加到v1后面
template <typename T>
void CombineVector(vector<T>& v1, vector<T>& v2)
{
	for(vector<T>::iterator it = v2.begin(); it != v2.end(); ++it)
	{
		v1.push_back(*it);
	}
}

//合并2个list,l2加到l1后面
template <typename T>
void CombineList(list<T>& l1, list<T>& l2)
{
	for(list<T>::iterator it = l2.begin(); it != l2.end(); ++it)
	{
		l1.push_back(*it);
	}
}

//合并2个list,l2加到l1后面,l1中已有元素不重复加入
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

//获取2维vector其中一个元素周围的元素
template <typename T>
void GetAroundFrom2DVector(int r, int c, vector<vector<T>>& vec, vector<T>& out)
{
	bool rp = r + 1 > vec.size() - 1 ? false : true; //可以往下
	bool rm = r - 1 < 0 ? false : true; //可以往上
	bool cp = c + 1 > vec.front().size() - 1 ? false : true; //可以往右
	bool cm = c - 1 < 0 ? false : true; //可以往左

	if( rm )
	{
		//上方
		out.push_back(vec[r - 1][c]);
		if( cm )
		{
			//左上角
			out.push_back(vec[r - 1][c - 1]);
		}
		if( cp )
		{
			//右上角
			out.push_back(vec[r - 1][c + 1]);
		}
	}
	if( rp )
	{
		//下方
		out.push_back(vec[r + 1][c]);
		if( cm )
		{
			//左下角
			out.push_back(vec[r + 1][c - 1]);
		}
		if( cp )
		{
			//右下角
			out.push_back(vec[r + 1][c + 1]);
		}
	}
	if( cm )
	{
		//左方
		out.push_back(vec[r][c - 1]);
	}
	if( cp )
	{
		//右方
		out.push_back(vec[r][c + 1]);
	}
}

void SP2SPEX(vector<StopPoint>& sp, vector<StopPointex>& spex); //停留点转带标记的停留点

double CalAverPointDis(double a, int n); //平均点距近似公式

#endif