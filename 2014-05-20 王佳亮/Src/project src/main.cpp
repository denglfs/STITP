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

	//读取停留点信息
	vector<StopPointex> spex;
	ReadSPEX(spex);
	printf("停留点信息读取完毕,共 %d 个停留点\n", spex.size());

	//提取簇
	vector<vector<StopPointex>> clusterout;
	elki.GenerateClusterAttr(spex, clusterout);
	printf("spex归簇完毕\n");

	//修正簇首尾点
	elki.FixCluster(spex, clusterout);
	printf("簇首尾点修正完毕\n");

	//获取opticsof属性
	ELKIProcess_OPTICSOF elki_opticsof;
	elki_opticsof.GenerateOPTICSOFAttr(spex, clusterout);
	printf("获取opticsof属性完毕\n");

	//去除异常点
	//ratio为自定的异常点去除的参数 就是停留点OF值的阈值 我们设为2
	double ratio;
	printf("ratio:\n");
	scanf("%lf", &ratio);
	elki_opticsof.FixCluster(ratio, spex, clusterout);
	printf("异常点去除完毕\n");

	//存文件
	OutputAllSPEX(spex);
	printf("带簇属性的停留点输出完毕\n");
}

void test_paper3()
{
	//读取停留点信息
	vector<StopPointex> spex;
	ReadSPEX(spex);
	printf("停留点信息读取完毕,共 %d 个停留点\n", spex.size());

	//生成簇序列
	tempfunc2(spex);
	printf("簇序列生成完毕\n");
}

void test_paper()
{
	SysOptions sysopt; //系统参数纪录 可以无视
	Logger logger; //日志纪录
	char infostr[128]; //信息字符串存储

	//设定用户数量
	//总共182个用户
	//若输入n则只对用户0~n-1进行处理
	printf("用户数量: ");
	scanf("%d", &sysopt.usernum);

	vector<StopPoint> allsp;

	//计算停留点
	sprintf(infostr, "开始计算停留点...\n");
	logger.Log(infostr);
	printf("%s", infostr);
	CalStopPoint(sysopt, logger, allsp);

	//转换存储结构StopPoint为StopPointex
	vector<StopPointex> tempallspex;
	SP2SPEX(allsp, tempallspex);

	//输出停留点到文件
	OutputAllSPEXLatLng(tempallspex);
	OutputAllSPEX(tempallspex);
	printf("停留点文件输出完毕\n");
}

bool SORTOPTICSCMPARVT(const StopPointex& a, const StopPointex& b)
{
	return (a.ArvT < b.ArvT);
}

void tempfunc2(vector<StopPointex>& allspex)
{
	//按用户,文件筛选
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

	//按ARVT排序
	for(int i = 0; i < vecspex.size(); ++i)
	{
		//每个用户
		for(vector<vector<StopPointex>>::iterator it = vecspex[i].begin(); it != vecspex[i].end(); ++it)
		{
			//每个PLT
			sort((*it).begin(), (*it).end(), SORTOPTICSCMPARVT);
		}
	}

	//写文件
	for(int i = 0; i < vecspex.size(); ++i)
	{
		//每个用户建立文件
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
				//每个PLT
				for(vector<StopPointex>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
				{
					//每个停留点
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
		//构造用户ID字符串
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

		//处理获取停留点
		TimeCounter spcounter;
		spcounter.Start();
		vector<StopPoint> sp;
		if( PFS_ProcessData(uid, sysopt, logger, sp) )
		{
			spcounter.End();
			sprintf(infostr, "用户 %d 停留点处理完毕,共 %d 个停留点,用时 %u ms\n", i, sp.size(), spcounter.GetCount());
			alltime += spcounter.GetCount();
			logger.Log(infostr);
			printf("%s", infostr);

			CombineVector(allsp, sp);
		}
	}

	sprintf(infostr, "共 %d 个停留点, 总用时 %u ms\n", allsp.size(), alltime);
	logger.Log(infostr);
	printf("%s", infostr);
}