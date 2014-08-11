#define _CRT_SECURE_NO_WARNINGS

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
	for(vector<wstring>::iterator it = files.begin(); it != files.end(); ++it)
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