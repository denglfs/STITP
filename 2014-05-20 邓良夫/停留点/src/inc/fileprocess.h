#ifndef _FILEPROCESS_H
#define _FILEPROCESS_H

#include <string>
#include <vector>

using namespace std;

#include <windows.h>

#include "global.h"

#define SIZE_READBUF 128

bool ReadFromFile(vector<GPSPoint>& gpspoint, wstring& uid);
bool GetGPSFiles(vector<wstring>& files, wstring& recid);
bool ReadGPSData(vector<wstring>& files, wstring& szuserid, vector<GPSPoint>& gpspoint);

bool WriteSPToFile(vector<StopPoint>& sp, LPCTSTR filename);


#endif