#ifndef _ELKI_H
#define _ELKI_H

#include "global.h"

class ELKIProcess_OPTICS
{
protected:
	long long GetNearestPointID(vector<StopPointex>& vecspex, StopPointex& p);

public:
	void GenerateOPTICS2DPlot(vector<StopPointex>& optics2dplotout); //����optics 2d plot����
	void GenerateClusterAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //��ȡ������
	void FixCluster(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //����ĳЩ����β�ĵ�ִش��������
};

class ELKIProcess_DBSCAN
{
public:
	void GenerateClusterAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //��ȡ������
};

class ELKIProcess_OPTICSOF
{
public:
	void GenerateOPTICSOFAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //��ȡOPTICSOF����
	void FixCluster(double ratio, vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //ȥ���쳣��
};

bool SortSPEXByOPTICSOF(const StopPointex& a, const StopPointex& b); //��OPTICSOF��������

#endif