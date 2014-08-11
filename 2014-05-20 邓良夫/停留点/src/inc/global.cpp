#define _CRT_SECURE_NO_WARNINGS

#include <cmath>

using namespace std;

#include "global.h"

TimeCounter::TimeCounter()
{
	m_dwStart = 0;
	m_dwEnd = 0;
}

void TimeCounter::Start()
{
	m_dwStart = GetTickCount();
}

void TimeCounter::End()
{
	m_dwEnd = GetTickCount();
}

DWORD TimeCounter::GetCount()
{
	return (m_dwEnd - m_dwStart);
}

Logger::Logger()
{
	m_strFile = "log ";

	time_t timet = time(NULL);
	tm* pt = localtime(&timet);
	char sztime[32];
	sprintf(sztime, "%d-%d-%d-%d-%d-%d", pt -> tm_year + 1900, pt -> tm_mon + 1, pt -> tm_mday, pt -> tm_hour, pt -> tm_min, pt -> tm_sec);

	m_strFile += sztime;
	m_strFile += ".txt";
}

bool Logger::Log(char* str)
{
	if( str )
	{
		FILE* fp = fopen(m_strFile.c_str(), "a+");
		if( fp )
		{
			fputs(str, fp);

			fclose(fp);

			return true;
		}
	}

	return false;
}

Point::Point()
{

}

Point::Point(double lat, double lng, double h, unsigned long user)
{
	Lat = lat;
	Lng = lng;
	H = h;
	UserID = user;
}

bool Point::operator ==(const Point& p) const
{
	if( fabs(Lat - p.Lat) < FLOAT_EQUAL && fabs(Lng - p.Lng) < FLOAT_EQUAL && fabs(H - p.H) < FLOAT_EQUAL )
	{
		return true;
	}

	return false;
}

GPSPoint::GPSPoint()
{

}

GPSPoint::GPSPoint(double lat, double lng, double h, time_t& t, unsigned long user)
{
	Point(lat, lng, h, user);
	T = t;
}

StopPoint::StopPoint()
{

}

StopPoint::StopPoint(double lat, double lng, double h, time_t& arvt, time_t& levt, unsigned long user)
{
	Point(lat, lng, h, user);
	ArvT = arvt;
	LevT = levt;
}

bool StopPoint::operator ==(const StopPoint& p) const
{
	const Point& rthis = *this;
	const Point& rp = p;
	if( rthis == rp && ArvT == p.ArvT && LevT == p.LevT )
	{
		return true;
	}

	return false;
}

StopPointex::StopPointex()
{
	CID = -1;
	State = STATE_SPEX_UNVISITED;
	ReachDis = REACHDIS_SPEX_UNDEFINED;
	CoreDis = COREDIS_SPEX_UNDEFINED;
	ID = ID_SPEX_UNDEFINED;
}

void StopPointex::SetSP(StopPoint& sp)
{
	StopPoint* p = (StopPoint*)this;
	*p = sp;
}

ShareCluster::ShareCluster()
{

}

ShareCluster::ShareCluster(long long c1, long long c2)
{
	ClusterID1 = c1;
	ClusterID2 = c2;
}

double DegToRad(double degree)
{
	return (degree / 180 * PI);
}

double RadToDeg(double radian)
{
	return (radian * 180 / PI);
}

double GetPointsDistance(Point& p1, Point& p2)
{
	double lat1 = p1.Lat;
	double lon1 = p1.Lng;
	double lat2 = p2.Lat;
	double lon2 = p2.Lng;
	if(abs(lat1-lat2) < 0.000001 || abs(lon1-lon2) < 0.000001)
		return 0;
	double theta = lon1 - lon2;
	double dist = sin(DegToRad(lat1)) * sin(DegToRad(lat2))
		+ cos(DegToRad(lat1)) * cos(DegToRad(lat2)) * cos(DegToRad(theta));
	dist = acos(dist);
	dist = RadToDeg(dist);

	return (dist * 60 * 1.1515 * 1.609344 * 1000.0);
}

bool IsEqualTM(const tm& t1, const tm& t2)
{
	if( t1.tm_hour != t2.tm_hour || t1.tm_min != t2.tm_min || t1.tm_sec != t2.tm_sec || t1.tm_year != t2.tm_year || 
		t1.tm_mday != t2.tm_mday || t1.tm_mon != t2.tm_mon )
	{
		return false;
	}

	return true;
}

void SP2SPEX(vector<StopPoint>& sp, vector<StopPointex>& spex)
{
	int i = 0;
	for(vector<StopPoint>::iterator it = sp.begin(); it != sp.end(); ++it, ++i)
	{
		StopPointex tspex;
		tspex.SetSP(*it);
		tspex.ID = i;
		spex.push_back(tspex);
	}
}