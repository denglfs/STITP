#ifndef _ELKI_H
#define _ELKI_H

#include "global.h"

class ELKIProcess_OPTICS
{
protected:
	long long GetNearestPointID(vector<StopPointex>& vecspex, StopPointex& p);

public:
	void GenerateOPTICS2DPlot(vector<StopPointex>& optics2dplotout); //生成optics 2d plot数据
	void GenerateClusterAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //获取簇属性
	void FixCluster(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //修正某些簇首尾的点分簇错误的问题
};

class ELKIProcess_DBSCAN
{
public:
	void GenerateClusterAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //获取簇属性
};

class ELKIProcess_OPTICSOF
{
public:
	void GenerateOPTICSOFAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //获取OPTICSOF属性
	void FixCluster(double ratio, vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout); //去除异常点
};

bool SortSPEXByOPTICSOF(const StopPointex& a, const StopPointex& b); //按OPTICSOF递增排序

#endif