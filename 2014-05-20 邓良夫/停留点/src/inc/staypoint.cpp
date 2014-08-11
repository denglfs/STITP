#include "staypoint.h"
#include <fstream>
#include <iomanip>
void writeGpsPointTofile(vector<GPSPoint>& gpspoint)
{
	ofstream outf("E:\\gpsPoint.txt");
	if (!outf)
	{
		cout<<"file open error!"<<endl;
		return ;
	}
	for(vector<GPSPoint>::iterator it1 = gpspoint.begin();it1!= gpspoint.end();it1++ )
	{
		outf<<setiosflags(ios::fixed)<<setprecision(14)<<it1->Lat<<" ";
		outf<<setiosflags(ios::fixed)<<setprecision(15)<<it1->Lng<<endl;
	}
	outf.close();
}
void writeStopPointTofile(vector<StopPoint>& out)
{
	ofstream outf("E:\\StopPoint.txt");
	if (!outf)
	{
		cout<<"file open error!"<<endl;
		return ;
	}
	for(vector<StopPoint>::iterator it1 = out.begin();it1!= out.end();it1++ )
	{
		outf<<setiosflags(ios::fixed)<<setprecision(14)<<it1->Lat<<" ";
		outf<<setiosflags(ios::fixed)<<setprecision(15)<<it1->Lng<<endl;
	}
	outf.close();
}
void print(vector<smallSP> & vsp)
{
	for(vector<smallSP>::iterator it = vsp.begin();it != vsp.end();it++)
	{
		it->getInfox();
		if (it ->type  == 1)
			printf("type = %d  (duration = %.2lf\tlenght = %.2lf) \t vthread = %lf \n",it->type,it->duration,it->length,it->volocity);
		else
			printf("type = %d  duration = %.2lf\tlenght = %.2lf \t vthread = %lf \n",it->type,it->duration,it->length,it->volocity);

		//getchar();
	}
}
void GPSP2SP(vector<GPSPoint>& gpspoint, double timelimit, double vthreshold, vector<StopPoint>& out)
{/*
	double pathLength =0;
	vector<GPSPoint>::iterator it = gpspoint.begin();
	for(int i=0;i<1160;i++)
	{
		pathLength += (GetPointsDistance(*(it),*(it+1)));
		it ++;
		printf("%d:\t%lf\n",i,pathLength);
	}*/
	vector<GPSPoint> tmp (gpspoint.begin(),gpspoint.begin()+1000);
	gpspoint = tmp;
	writeGpsPointTofile(tmp);
	bool* isnext = new bool[gpspoint.size() - 1];
	vector<smallSP> smallSPList;//停留点序列的初次分片结构
	ProcessNext(gpspoint, isnext,vthreshold);
	cout<<"isnext is ok.."<<endl;
	paragraphing(gpspoint,smallSPList,isnext);//停留点序列的初次分片
	
	smallCombine(smallSPList,20,30);
	//print(smallSPList);
	MakeSP(smallSPList,out);
	writeStopPointTofile(out);


	SAFE_DELETE_ARRAY(isnext);
}



void paragraphing(vector<GPSPoint>& gpspoint, vector<smallSP> &smallSPList,bool* isnext)
{
		vector<GPSPoint>::iterator begin = gpspoint.begin();
		vector<GPSPoint>::iterator end = begin;
		int beginnum = 0;
		int endnum = 0;
		bool mark = false;
		int times = 0;
		int i=0;
		for(vector<GPSPoint>::iterator it = gpspoint.begin(); it != gpspoint.end() - 1; ++it, ++i)
		{
			if( isnext[i + 1] != isnext[i] )
			{
				end = it;
				vector<GPSPoint> tmplist(begin,end+2);
				smallSP tmpssp;
				tmpssp.type = isnext[i];
				tmpssp.list = tmplist;
				tmpssp.getInfo();
				smallSPList.push_back(tmpssp);
				times = 0;
				begin = end +1;
			}

		}
		
}

void ProcessNext(vector<GPSPoint>& gpspoint, bool* isnext, double vthreshold)
{
	int i = 0;
	for(vector<GPSPoint>::iterator it = gpspoint.begin(); it != gpspoint.end() - 1; ++it, ++i)
	{
		double dis = GetPointsDistance(*it, *(it + 1));
		double v = dis / ((*(it + 1)).T - (*it).T);
		if( v <= vthreshold )
		{
			isnext[i] = false;
		}
		else
		{
			isnext[i] = true;
		}
	}

	/*for(int k = 0; k < 1000; ++k)
	{
		printf("%d ", isnext[k]);
	}*/
}

void smallCombine(vector<smallSP> &smallSPList, double moveTimelimit,double stopTimelimit)
{
	vector<smallSP> tmp;
	vector<smallSP> tmpx=smallSPList;
	while(1)
	{
		int num1=tmpx.size();
		for(vector<smallSP>::iterator it = tmpx.begin();it <= tmpx.end()-2;it++)
		{
			if(it->type == 0)//stop state
			{
				if ( it->duration < stopTimelimit )
				{
					it->type = 1;
					it->combineTwoSmallSP(*(it+1));
					tmp.push_back(*it);
					++it;
				}
				else
					tmp.push_back(*it);
			}
			else//move state
			{
				if ( it->duration < moveTimelimit )
				{
					it->type = 0;
					it->combineTwoSmallSP(*(it+1));
					tmp.push_back(*it);
					++it;
				}
				else
					tmp.push_back(*it);
			}
		}
		tmpx.clear();
		vector<smallSP>::iterator itx = tmp.begin();
		while(itx < tmp.end() - 1)
		{
			smallSP stmp;
			for(;itx < tmp.end()-1 && itx->type == (itx+1)->type;itx++)
			{
				stmp.combineTwoSmallSP(*itx);
			}
			stmp.combineTwoSmallSP(*itx);
			stmp.type = itx->type;
			
			tmpx.push_back(stmp);
			++itx;
		}
		int num2=tmpx.size();
		tmp.clear();
		if(num1-num2 < 10 )
		{
			break;
		}
		
		
	}
	int index=0;
	for(vector<smallSP>::iterator it1=tmpx.begin();it1 != tmpx.end();it1++)
	{
		it1->getInfox();
		
		/*if(it1->detour >=3 && it1->duration > 120)
		{
			index++;
			printf("%d\t%lf\t%lf\t%lf\t%lf\n",index,it1->duration,it1->length,it1->pathLength,it1->detour);
		}*/
	}

	while(1)
	{
		int num1=tmpx.size();
		for(vector<smallSP>::iterator it = tmpx.begin();it <= tmpx.end()-2;it++)
		{
			if(it->type == 0)//stop state
			{
				/*if ( it->isSP(t,d) == false )
				{
					it->type = 1;
					it->combineTwoSmallSP(*(it+1));
					tmp.push_back(*it);
					++it;
				}
				else*/
					tmp.push_back(*it);
			}
			else//move state
			{
				if ( it->isMP(l,d) == false )
				{
					it->type = 0;
					it->combineTwoSmallSP(*(it+1));
					tmp.push_back(*it);
					++it;
				}
				else
					tmp.push_back(*it);
			}
		}
		tmpx.clear();
		vector<smallSP>::iterator itx = tmp.begin();
		while(itx < tmp.end() - 1)
		{
			
			smallSP stmp;
			for(;itx < tmp.end()-1 && itx->type == (itx+1)->type;itx++)
			{
				stmp.combineTwoSmallSP(*itx);
			}
			stmp.combineTwoSmallSP(*itx);
			stmp.type = itx->type;
			stmp.getInfo();
			stmp.getInfox();
			/*for(int i =0;i<1600;i++)
			{
				if( stmp.list[i].Lat > 180 || stmp.list[i].Lat < 0 || stmp.list[i].Lng < 0  || stmp.list[i].Lng > 180)
				{
					system("pause");
				}
			}*/
			tmpx.push_back(stmp);
			++itx;
		}
		int num2=tmpx.size();
		tmp.clear();
		if(num1-num2 < 10)
		{
			break;
		}
		
		
	}
	index = 0;
	for(vector<smallSP>::iterator it1=tmpx.begin();it1 != tmpx.end();it1++)
	{
		it1->getInfox();
		
		/*if(it1->detour >=3 && it1->duration > 120)
		{
			index++;
			printf("%d\t%lf\t%lf\t%lf\t%lf\n",index,it1->duration,it1->length,it1->pathLength,it1->detour);
		}*/
	}
	printf("after smallsize combine.. total num ：%d \t\n",tmpx.size());
	smallSPList = tmpx;
	//system("pause");
}

void MakeSP(vector<smallSP>& vecsp,vector<StopPoint>& out)
{
	char stopFileName[32];
	int i =0;
	for(vector<smallSP>::iterator it = vecsp.begin();it!= vecsp.end();it++)
	{
		if (it ->type == 0)
		{
			sprintf(stopFileName,"E:\\StopFile%d.txt\0",i);i++;
			ofstream outf(stopFileName);
			StopPoint tmpsp;
			tmpsp.ArvT = it->list.begin()->T;
			tmpsp.LevT = (it->list.end() -1 ) -> T;
			tmpsp.UserID = it->list.begin() ->UserID;
			double totalLng = 0;
			double totalLat = 0;
			double totalH =0;
			for(vector<GPSPoint>::iterator itgpspoint = it->list.begin();itgpspoint != it->list.end();itgpspoint ++)
			{
				outf<<setiosflags(ios::fixed)<<setprecision(14)<<itgpspoint->Lat<<" ";
				outf<<setiosflags(ios::fixed)<<setprecision(15)<<itgpspoint->Lng<<endl;
				totalH += itgpspoint->H;
				totalLat += itgpspoint->Lat;
				totalLng += itgpspoint->Lng;
			}
			int tmpsize = it->list.size();
			tmpsp.Lat = totalLat/tmpsize;
			tmpsp.Lng = totalLng/tmpsize;
			tmpsp.H = totalH /tmpsize;
			out.push_back(tmpsp);

			outf.close();
		}
	}
}
