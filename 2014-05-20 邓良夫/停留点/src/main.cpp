#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>

using namespace std;

#include "inc\global.h"
#include "inc\staypoint.h"
#include "inc\fileprocess.h"


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





void testall();
void MakeStopPoints(SysOptions& sysopt, Logger& logger, vector<StopPoint>& allsp);
void WriteOptionToFile(SysOptions& opt);

int main()
{
	while( true )
	{
		testall();
	}

	system("pause");

	return 0;
}



void testall()
{
	SysOptions sysopt;
	Logger logger;


	//�û�����
	printf("numuser:\n");
	scanf("%d", &sysopt.usernum);

	//����ͣ����
	vector<StopPoint> allsp;
	MakeStopPoints(sysopt, logger, allsp);

	
	WriteOptionToFile(sysopt);
}

void WriteOptionToFile(SysOptions& opt)
{
	FILE* fp = fopen("opt.txt", "w");
	if( fp )
	{
		fprintf(fp, "%d %f %f %f %u %d %f\n", opt.usernum, opt.timelimit, opt.vthreshold, opt.eps, opt.minpts, opt.uid, opt.p);

		fclose(fp);
	}
}

void MakeStopPoints(SysOptions& sysopt, Logger& logger, vector<StopPoint>& allsp)
{
	char infostr[128];
	printf("timelimit vthreshold:");
	scanf("%lf %lf", &sysopt.timelimit, &sysopt.vthreshold);
	DWORD alltime = 0;
	wstring uid;
	WCHAR wnum[4];

	//����ͣ�����㷨ȱ�� �����û����ݴ������
	//v=1.5 49 54
	//v=1.2 123 149
	//v=0.8 49 90

	for(int i = 0; i < sysopt.usernum; ++i)
	{
		//�����û�ID�ַ���
		_itow(i, wnum, 10);
		if( i < 10 )
		{
			uid = L"00";
			uid += wnum;
		}
		else if( i < 100 )
		{
			uid = L"0";
			uid += wnum;
		}
		else
		{
			uid = wnum;
		}

		//����GPS�� ת��ͣ����
		vector<GPSPoint> gpspoint;
		TimeCounter readfilecounter;
		readfilecounter.Start();
		if( ReadFromFile(gpspoint, uid) )
		{
			readfilecounter.End();

			vector<StopPoint> sp;
			TimeCounter g2scounter;
			g2scounter.Start();
			GPSP2SP(gpspoint, sysopt.timelimit, sysopt.vthreshold, sp);
			g2scounter.End();

			wstring outfile = uid + L".txt";
			WriteSPToFile(sp, outfile.c_str());

			sprintf(infostr, "�û� %d �������,��ȡ�ļ���ʱ %u ms,��ЧGPS�� %d,ͣ���� %d,��ʱ %u ms\n", i, readfilecounter.GetCount(), 
				gpspoint.size(), sp.size(), g2scounter.GetCount());
			logger.Log(infostr);
			printf("%s", infostr);

			CombineVector(allsp, sp);
			alltime = alltime + readfilecounter.GetCount() + g2scounter.GetCount();
		}
	}

	sprintf(infostr, "����ͣ���� %d ��, ������ʱ %u ms\n", allsp.size(), alltime);
	logger.Log(infostr);
	printf("%s", infostr);
}
