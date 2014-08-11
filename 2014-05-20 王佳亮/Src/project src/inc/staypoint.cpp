#include "staypoint.h"
#include <fstream>
#include <iomanip>
//void writedatatofile(vector<GPSPoint>& gpspoint)
//{
//	ofstream outfx("E:\\x.txt");
//	for(int i=0;i<gpspoint.size();i++)
//		outfx<<"1"<<endl;
//	outfx.close();
//	ofstream outf("E:\\lat.txt");
//	for(vector<GPSPoint>::iterator it1 = gpspoint.begin();it1!= gpspoint.end();it1++ )
//	{
//		outf<<setiosflags(ios::fixed)<<setprecision(14)<<it1->Lat<<endl;
//	}
//	outf.close();
//
//	ofstream outf1("E:\\lng.txt");
//	for(vector<GPSPoint>::iterator it2 = gpspoint.begin();it2!= gpspoint.end();it2++ )
//	{
//		outf1<<setiosflags(ios::fixed)<<setprecision(15)<<it2->Lng<<endl;
//	}
//	outf1.close();
//}
void writedatatofile(vector<StopPoint>& out)
{
	ofstream outf("E:\\splat.txt");
	for(vector<StopPoint>::iterator it1 = out.begin();it1!= out.end();it1++ )
	{
		outf<<setiosflags(ios::fixed)<<setprecision(14)<<it1->Lat<<endl;
	}
	outf.close();

	ofstream outf1("E:\\splng.txt");
	for(vector<StopPoint>::iterator it2 = out.begin();it2!= out.end();it2++ )
	{
		outf1<<setiosflags(ios::fixed)<<setprecision(15)<<it2->Lng<<endl;
	}
	outf1.close();
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
{
	vthreshold = 0.6;
	/*
	double pathLength =0;
	vector<GPSPoint>::iterator it = gpspoint.begin();
	for(int i=0;i<1160;i++)
	{
		pathLength += (GetPointsDistance(*(it),*(it+1)));
		it ++;
		printf("%d:\t%lf\n",i,pathLength);
	}*/
	//vector<GPSPoint> tmp (gpspoint.begin(),gpspoint.begin()+2000);
	//gpspoint = tmp;
	bool* isnext = new bool[gpspoint.size() - 1];
	vector<smallSP> smallSPList;//停留点序列的初次分片结构
	ProcessNext(gpspoint, isnext, vthreshold);
//	cout<<"isnext is ok.."<<endl;
	paragraphing(gpspoint,smallSPList,isnext);//停留点序列的初次分片
	
	smallCombine(smallSPList,20,30);
	//print(smallSPList);
	MakeSP(smallSPList,out);
	writedatatofile(out);
	//int ret = Combine(gpspoint, isnext, timelimit / 1000);
	/*if( ret == 0 )
	{
		MakeSP(gpspoint, isnext, out);
	}*/
	/*else if( ret == 1 )
	{
		for(vector<GPSPoint>::iterator it = gpspoint.begin(); it != gpspoint.end() - 1; ++it)
		{
			StopPoint tsp;
			tsp.Lat = (*it).Lat;
			tsp.Lng = (*it).Lng;
			tsp.H = (*it).H;
			tsp.ArvT = (*it).T;
			tsp.LevT = (*(it + 1)).T;
			tsp.UserID = (*it).UserID;
			out.push_back(tsp);
		}
	}*/

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
				/*++times;
				if( !mark )
				{
					mark = true;
					begin = it + 1;
					beginnum = i + 1;
				}
				else
				{
					mark = false;
					end = it;
					endnum = i;
				}*/
			}
			/*if( !(times % 2) && times )
			{
				vector<GPSPoint> tmplist(begin,end+2);
				smallSP tmpssp;
				tmpssp.type = isnext[i];
				tmpssp.list = tmplist;
				tmpssp.getInfo();
				smallSPList.push_back(tmpssp);
				times = 0;
				
			}*/
		}
		/*int j = 0;
		for(vector<smallSP>::iterator  it =  smallSPList.begin();it != smallSPList.end();it ++ )
		{
			j++;
			if(j> 100)
				break;
			using namespace std;
			it->getInfo();
			cout<<it->duration<<"\t\t"<<it->length<<"\t\t\t"<<it->pointNum<<endl;

		}*/
		//system("pause");
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
		if ( num1 <= 2 )
			break;
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
	const double d = 2;
	const double t = 120 ;
	const double l = 140;
	while(1)
	{
		int num1=tmpx.size();
		if(num1 <= 2)
			break;
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
//	printf("after smallsize combine.. total num ：%d \t\n",tmpx.size());
	smallSPList = tmpx;
	//system("pause");
}

void MakeSP(vector<smallSP>& vecsp,vector<StopPoint>& out)
{
	for(vector<smallSP>::iterator it = vecsp.begin();it!= vecsp.end();it++)
	{
		if (it ->type == 0)
		{
			StopPoint tmpsp;
			tmpsp.ArvT = it->list.begin()->T;
			tmpsp.LevT = (it->list.end() -1 ) -> T;
			tmpsp.UserID = it->list.begin() ->UserID;
			double totalLng = 0;
			double totalLat = 0;
			double totalH =0;
			for(vector<GPSPoint>::iterator itgpspoint = it->list.begin();itgpspoint != it->list.end();itgpspoint ++)
			{
				totalH += itgpspoint->H;
				totalLat += itgpspoint->Lat;
				totalLng += itgpspoint->Lng;
			}
			int tmpsize = it->list.size();
			tmpsp.Lat = totalLat/tmpsize;
			tmpsp.Lng = totalLng/tmpsize;
			tmpsp.H = totalH /tmpsize;
			out.push_back(tmpsp);
		}
	}
}

//int Combine(vector<GPSPoint>& gpspoint, bool* isnext, double timelimit)
//{
//	bool breaksign;
//	vector<GPSPoint>::iterator begin = gpspoint.begin();
//	vector<GPSPoint>::iterator end = begin;
//	int beginnum = 0;
//	int endnum = 0;
//	bool mark = false;
//	int times = 0;
//	while( true )
//	{
//		breaksign = false;
//		int i = 0;
//		for(vector<GPSPoint>::iterator it = gpspoint.begin(); it != gpspoint.end() - 1; ++it, ++i)
//		{
//			if( isnext[i + 1] != isnext[i] )
//			{
//				++times;
//				if( !mark )
//				{
//					mark = true;
//					begin = it + 1;
//					beginnum = i + 1;
//				}
//				else
//				{
//					mark = false;
//					end = it;
//					endnum = i;
//				}
//			}
//			if( !(times % 2) && times )
//			{
//				if( (*end).T - (*begin).T + 1 < timelimit )
//				{
//					breaksign = true;
//					for(int j = beginnum; j <= endnum; ++j)
//					{
//						isnext[j] = isnext[beginnum - 1];
//					}
//				}
//			}
//		}
//
//		if( !breaksign )
//		{
//			break;
//		}
//
//		bool has0 = false;
//		for(int i = 0; i < gpspoint.size() - 1; ++i)
//		{
//			if( !isnext[i] )
//			{
//				has0 = true;
//
//				break;
//			}
//		}
//		if( !has0 )
//		{
//			break;
//		}
//
//		检测全0和全1的情况 全0=每个都是停留点 全1=没有停留点
//		bool have0 = false;
//		bool have1 = false;
//		for(int k = 0; k < gpspoint.size() - 1; ++k)
//		{
//			if( isnext[k] == false )
//			{
//				have0 = true;
//			}
//			else
//			{
//				have1 = true;
//			}
//			if( have0 && have1 )
//			{
//				break;
//			}
//		}
//		if( have0 && !have1 )
//		{
//			return 1;
//		}
//		if( !have0 && have1 )
//		{
//			return 2;
//		}
//	}
//
//	return 0;
//}
//
//void MakeSP(vector<GPSPoint>& gpspoint, bool* isnext, vector<StopPoint>& out)
//{
//	bool isdeal = true;
//	int i = 0;
//	for(vector<GPSPoint>::iterator it = gpspoint.begin(); it != gpspoint.end() - 1; ++it, ++i)
//	{
//		double tx = 0;
//		double ty = 0;
//		int tnum = 0;
//		while( i < gpspoint.size() - 1 && !isnext[i] )
//		{
//			isdeal = false;
//			tx += (*it).Lat;
//			ty += (*it).Lng;
//			++i;
//			++it;
//			++tnum;
//		}
//		if( !isdeal )
//		{
//			--i;
//			--it;
//			StopPoint tsp;
//			tsp.Lat = tx / tnum;
//			tsp.Lng = ty / tnum;
//			tsp.H = -1;
//			tsp.ArvT = (*(it + 1 - tnum)).T;
//			tsp.LevT = (*it).T;
//			tsp.UserID = (*it).UserID;
//			out.push_back(tsp);
//			isdeal = true;
//		}
//	}
//}