#include <cstdio>

using namespace std;

#include "inc\main.h"

#include "inc\staypoint.h"
#include "inc\fileprocess.h"

#include "inc\elki.h"

int main()
{
//	test_paper();
//	test_paper2();
//	test_paper3();

	system("pause");

	return 0;
}

void test_paper2()
{
	ELKIProcess_OPTICS elki;

	//��ȡͣ������Ϣ
	vector<StopPointex> spex;
	ReadSPEX(spex);
	printf("ͣ������Ϣ��ȡ���,�� %d ��ͣ����\n", spex.size());

	//��ȡ��
	vector<vector<StopPointex>> clusterout;
	elki.GenerateClusterAttr(spex, clusterout);
	printf("spex������\n");

	//��������β��
	elki.FixCluster(spex, clusterout);
	printf("����β���������\n");

	//��ȡopticsof����
	ELKIProcess_OPTICSOF elki_opticsof;
	elki_opticsof.GenerateOPTICSOFAttr(spex, clusterout);
	printf("��ȡopticsof�������\n");

	//ȥ���쳣��
	//ratioΪ�Զ����쳣��ȥ���Ĳ��� ����ͣ����OFֵ����ֵ ������Ϊ2
	double ratio;
	printf("ratio:\n");
	scanf("%lf", &ratio);
	elki_opticsof.FixCluster(ratio, spex, clusterout);
	printf("�쳣��ȥ�����\n");

	//���ļ�
	OutputAllSPEX(spex);
	printf("�������Ե�ͣ����������\n");
}

void test_paper3()
{
	//��ȡͣ������Ϣ
	vector<StopPointex> spex;
	ReadSPEX(spex);
	printf("ͣ������Ϣ��ȡ���,�� %d ��ͣ����\n", spex.size());

	//���ɴ�����
	tempfunc2(spex);
	printf("�������������\n");
}

void test_paper()
{
	SysOptions sysopt; //ϵͳ������¼ ��������
	Logger logger; //��־��¼
	char infostr[128]; //��Ϣ�ַ����洢

	//�趨�û�����
	//�ܹ�182���û�
	//������n��ֻ���û�0~n-1���д���
	printf("�û�����: ");
	scanf("%d", &sysopt.usernum);

	vector<StopPoint> allsp;

	//����ͣ����
	sprintf(infostr, "��ʼ����ͣ����...\n");
	logger.Log(infostr);
	printf("%s", infostr);
	CalStopPoint(sysopt, logger, allsp);

	//ת���洢�ṹStopPointΪStopPointex
	vector<StopPointex> tempallspex;
	SP2SPEX(allsp, tempallspex);

	//���ͣ���㵽�ļ�
	OutputAllSPEXLatLng(tempallspex);
	OutputAllSPEX(tempallspex);
	printf("ͣ�����ļ�������\n");
}

bool SORTOPTICSCMPARVT(const StopPointex& a, const StopPointex& b)
{
	return (a.ArvT < b.ArvT);
}

void tempfunc2(vector<StopPointex>& allspex)
{
	//���û�,�ļ�ɸѡ
	vector<vector<vector<StopPointex>>> vecspex(182);
	for(vector<StopPointex>::iterator it = allspex.begin(); it != allspex.end(); ++it)
	{
		StopPointex tspex = *it;
		if( vecspex[tspex.UserID].size() < tspex.FileID + 1 )
		{
			vecspex[tspex.UserID].resize(tspex.FileID + 1);
		}
		vecspex[tspex.UserID][tspex.FileID].push_back(*it);
	}

	//��ARVT����
	for(int i = 0; i < vecspex.size(); ++i)
	{
		//ÿ���û�
		for(vector<vector<StopPointex>>::iterator it = vecspex[i].begin(); it != vecspex[i].end(); ++it)
		{
			//ÿ��PLT
			sort((*it).begin(), (*it).end(), SORTOPTICSCMPARVT);
		}
	}

	//д�ļ�
	for(int i = 0; i < vecspex.size(); ++i)
	{
		//ÿ���û������ļ�
		wstring filepath;
		wchar_t wnum[4];
		_itow(i, wnum, 10);
		filepath = wnum;
		filepath.append(L"_seq.txt");
		FILE* fp = _wfopen(filepath.c_str(), L"wt");
		if( fp )
		{
			for(vector<vector<StopPointex>>::iterator it = vecspex[i].begin(); it != vecspex[i].end(); ++it)
			{
				//ÿ��PLT
				for(vector<StopPointex>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
				{
					//ÿ��ͣ����
					fprintf(fp, "%d ", (*it2).CID);
				}
				if( (*it).size() )
				{
					fprintf(fp, "\n");
				}
			}
			fclose(fp);
		}
	}
}

void OutputAllSPEX(vector<StopPointex>& allspex)
{
	FILE* fp = fopen("allspex", "wb");
	if( fp )
	{
		for(vector<StopPointex>::iterator it = allspex.begin(); it != allspex.end(); ++it)
		{
			fwrite(&(*it), sizeof(StopPointex), 1, fp);
		}

		fclose(fp);
	}
}

void OutputAllSPEXLatLng(vector<StopPointex>& allspex)
{
	FILE* fp = fopen("allspex_latlng.txt", "wt");
	if( fp )
	{
		for(vector<StopPointex>::iterator it = allspex.begin(); it != allspex.end(); ++it)
		{
			fprintf(fp, "%f %f\n", (*it).Lng, (*it).Lat);
		}

		fclose(fp);
	}
}

void ReadSPEXLatLng(vector<StopPointex>& spex)
{
	FILE* fp = fopen("allspex_latlng.txt", "rt");
	if( fp )
	{
		StopPointex tspex;
		int spexid = 0;
		while( !feof(fp) )
		{
			if( fscanf(fp, "%lf %lf\n", &tspex.Lng, &tspex.Lat) == EOF )
			{
				break;
			}

			tspex.ID = spexid;
			spex.push_back(tspex);
			++spexid;
		}

		fclose(fp);
	}
}

void ReadSPEX(vector<StopPointex>& spex)
{
	FILE* fp = fopen("allspex2", "rb");
	if( fp )
	{
		StopPointex tsp;
		while( !feof(fp) )
		{
			if( fread(&tsp, sizeof(StopPointex), 1, fp) != 1 )
			{
				break;
			}

			spex.push_back(tsp);
		}
		fclose(fp);
	}
}

void CalStopPoint(SysOptions& sysopt, Logger& logger, vector<StopPoint>& allsp)
{
	char infostr[128];

	wstring uid;
	WCHAR wnum[4];

	DWORD alltime = 0;
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

		//�����ȡͣ����
		TimeCounter spcounter;
		spcounter.Start();
		vector<StopPoint> sp;
		if( PFS_ProcessData(uid, sysopt, logger, sp) )
		{
			spcounter.End();
			sprintf(infostr, "�û� %d ͣ���㴦�����,�� %d ��ͣ����,��ʱ %u ms\n", i, sp.size(), spcounter.GetCount());
			alltime += spcounter.GetCount();
			logger.Log(infostr);
			printf("%s", infostr);

			CombineVector(allsp, sp);
		}
	}

	sprintf(infostr, "�� %d ��ͣ����, ����ʱ %u ms\n", allsp.size(), alltime);
	logger.Log(infostr);
	printf("%s", infostr);
}