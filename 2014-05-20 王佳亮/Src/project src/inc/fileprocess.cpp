#include "main.h"
#include "staypoint.h"
#include "fileprocess.h"

bool ReadFromFile(vector<GPSPoint>& gpspoint, wstring& uid)
{
	vector<wstring> files;
	if( !GetGPSFiles(files, uid) )
	{
		return false;
	}
	if( !ReadGPSData(files, uid, gpspoint) )
	{
		return false;
	}

	return true;
}

bool GetGPSFiles(vector<wstring>& files, wstring& recid)
{
	wstring path = recid + L"\\Trajectory\\*.plt";
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(path.c_str(), &FindFileData);
    if( INVALID_HANDLE_VALUE == hFind )
    {
		printf("FindFirstFile()失败,错误代码%d\n", GetLastError());

		return false;
	}

	wstring filename;
	while( true )
    {
        if( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
        {
			filename = FindFileData.cFileName;
			files.push_back(filename);
        }

        if( !FindNextFile(hFind, &FindFileData) )
		{
			break;
		}
    }
    FindClose(hFind);

	return true;
}

bool ReadGPSData(vector<wstring>& files, wstring& szuserid, vector<GPSPoint>& gpspoint)
{
	int uid = _wtoi(szuserid.c_str());
	int fileid = 0;
	for(vector<wstring>::iterator it = files.begin(); it != files.end(); ++it, ++fileid)
	{
		wstring path = szuserid + L"\\Trajectory\\" + (*it);
		FILE* fp = _wfopen(path.c_str(), L"rt");
		if( fp )
		{
			//跳过前6行
			for(int i = 0; i < 6; ++i)
			{
				char buf[SIZE_READBUF];
				fgets(buf, SIZE_READBUF, fp);
			}

			GPSPoint tp;
			int tnum;
			double dayscount;
			time_t lastt = 0;
			while( !feof(fp) )
			{
				int year, month, day, hour, minute, second;
				if( fscanf(fp, "%lf,%lf,%d,%lf,%lf,%d-%d-%d,%d:%d:%d", &tp.Lat, &tp.Lng, &tnum, &tp.H, &dayscount, &year, &month, &day, &hour, &minute, &second) != -1 )
				{
					tm tmstruct;
					memset(&tmstruct, 0, sizeof(tm));
					tmstruct.tm_year = year - 1900;
					tmstruct.tm_mon = month - 1;
					tmstruct.tm_mday = day;
					tmstruct.tm_hour = hour;
					tmstruct.tm_min = minute;
					tmstruct.tm_sec = second;
					tmstruct.tm_isdst = 0;
					tp.T = mktime(&tmstruct);

					if( lastt == tp.T )
					{
						continue;
					}

					lastt = tp.T;
					tp.UserID = uid;
					tp.FileID = fileid;
					gpspoint.push_back(tp);
				}
			}

			fclose(fp);
		}
		else
		{
			printf("%ls 打开失败\n", path.c_str());

			return false;
		}
	}

	return true;
}

bool WriteSPToFile(vector<StopPoint>& sp, LPCTSTR filename)
{
	FILE* fp = _wfopen(filename, L"wt");
	if( fp )
	{
		for(vector<StopPoint>::iterator it = sp.begin(); it != sp.end(); ++it)
		{
			fprintf(fp, "%f %f %lld %lld\n", (*it).Lat, (*it).Lng, (*it).ArvT, (*it).LevT);
		}

		fclose(fp);
	}
	else
	{
		printf("%ls 打开失败\n", filename);

		return false;
	}

	return true;
}

bool PFS_ProcessData(wstring& recid, SysOptions& sysopt, Logger& logger, vector<StopPoint>& spout)
{
	wstring path = recid + L"\\Trajectory\\*.plt";
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(path.c_str(), &FindFileData);
    if( INVALID_HANDLE_VALUE == hFind )
    {
		printf("FindFirstFile()失败,错误代码%d\n", GetLastError());

		return false;
	}

	wstring filename;
	bool failflag = false;
	int fileid = 0;
	while( true )
    {
        if( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
        {
			//对一个PLT进行处理
			filename = FindFileData.cFileName;

			//获取GPS点
			vector<GPSPoint> gpspoint;
			if( !PFS_ReadGPSData(filename, recid, gpspoint) )
			{
				failflag = true;

				break;
			}

			//转换停留点
			vector<StopPoint> sp;
			GPSP2SP(gpspoint, sysopt.timelimit, sysopt.vthreshold, sp);
			for(vector<StopPoint>::iterator it = sp.begin(); it != sp.end(); ++it)
			{
				(*it).FileID = fileid;
			}

			CombineVector(spout, sp);

			++fileid;

			////聚类并获取簇序列
			//vector<vector<Cluster>> clusterout;
			//vector<StopPointex> allspex;
			//PFS_DBSCANClustering(sysopt, sp, allspex, seqout);
        }

        if( !FindNextFile(hFind, &FindFileData) )
		{
			break;
		}
    }
    FindClose(hFind);

	if( failflag )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool PFS_ReadGPSData(wstring& filename, wstring& szuserid, vector<GPSPoint>& gpspoint)
{
	int uid = _wtoi(szuserid.c_str());
	wstring path = szuserid + L"\\Trajectory\\" + filename;
	FILE* fp = _wfopen(path.c_str(), L"rt");
	if( fp )
	{
		//跳过前6行
		for(int i = 0; i < 6; ++i)
		{
			char buf[SIZE_READBUF];
			fgets(buf, SIZE_READBUF, fp);
		}

		GPSPoint tp;
		int tnum;
		double dayscount;
		time_t lastt = 0;
		while( !feof(fp) )
		{
			int year, month, day, hour, minute, second;
			if( fscanf(fp, "%lf,%lf,%d,%lf,%lf,%d-%d-%d,%d:%d:%d", &tp.Lat, &tp.Lng, &tnum, &tp.H, &dayscount, &year, &month, &day, &hour, &minute, &second) != -1 )
			{
				tm tmstruct;
				memset(&tmstruct, 0, sizeof(tm));
				tmstruct.tm_year = year - 1900;
				tmstruct.tm_mon = month - 1;
				tmstruct.tm_mday = day;
				tmstruct.tm_hour = hour;
				tmstruct.tm_min = minute;
				tmstruct.tm_sec = second;
				tmstruct.tm_isdst = 0;
				tp.T = mktime(&tmstruct);

				if( lastt == tp.T )
				{
					continue;
				}

				lastt = tp.T;
				tp.UserID = uid;
				gpspoint.push_back(tp);
			}
		}

		fclose(fp);
	}
	else
	{
		printf("%ls 打开失败\n", path.c_str());

		return false;
	}

	return true;
}

bool PFS_WriteSeq(wstring& uid, vector<int>& seq)
{
	wstring path = uid + L"_seq.txt";
	FILE* fp = _wfopen(path.c_str(), L"wt");
	if( fp )
	{
		for(vector<int>::iterator it = seq.begin(); it != seq.end(); ++it)
		{
			fprintf(fp, "%d ", *it);
		}

		fclose(fp);

		return true;
	}
	else
	{
		printf("%ls 打开失败\n", path.c_str());

		return false;
	}
}