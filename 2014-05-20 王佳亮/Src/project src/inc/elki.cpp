#include <Windows.h>

#include "elki.h"

long long ELKIProcess_OPTICS::GetNearestPointID(vector<StopPointex>& vecspex, StopPointex& p)
{
	long long id = -1;
	double mindis = -1;
	for(int i = 0; i < vecspex.size(); ++i)
	{
		if( vecspex[i].ID != p.ID )
		{
			double dis = GetPointsDistance(p, vecspex[i]);
			if( mindis == -1 || dis < mindis )
			{
				mindis = dis;
				id = vecspex[i].ID;
			}
		}
	}

	return id;
}

void ELKIProcess_OPTICS::GenerateOPTICS2DPlot(vector<StopPointex>& optics2dplotout)
{
	FILE* fp = fopen("clusterobjectorder.txt", "rt");
	if( fp )
	{
		while( !feof(fp) )
		{
			char buf[128];
			if( !fgets(buf, 127, fp) )
			{
				break;
			}

			//���ҿɴ����λ��
			string thisline(buf);
			char rstr[] = "reachability";
			int findres = thisline.find(rstr, 0);
			if( findres == string::npos )
			{
				break;
			}
			
			StopPointex tspex;
			int valuestart = findres + strlen(rstr) + 1;
			if( buf[valuestart] == -95 && buf[valuestart + 1] == -34 )
			{
				//�ɴ����������
				tspex.ReachDis = REACHDIS_SPEX_UNDEFINED;
			}
			else
			{
				//�ҵ��ո��λ��
				int spacepos = thisline.find_last_of(" ");
				if( spacepos == string::npos )
				{
					printf("ELKIProcess_OPTICS::GenerateOPTICS2DPlot() ����\n");
					system("pause");
				}

				//����ɴ����
				string rstring(thisline, valuestart, spacepos - valuestart);
				tspex.ReachDis = atof(rstring.c_str());
			}
			optics2dplotout.push_back(tspex);
		}

		fclose(fp);
	}
}

void ELKIProcess_OPTICS::GenerateClusterAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout)
{
	list<string> vecchild;
	string firstfile("cluster");
	vecchild.push_back(firstfile);

	int cid = -1;
	for(list<string>::iterator it = vecchild.begin(); it != vecchild.end(); )
	{
		vector<StopPointex> vecthisc;

		string filename(*it);
		filename.append(".txt");
		FILE* fp = fopen(filename.c_str(), "rt");
		if( fp )
		{
			int linecounter = 0;
		
			bool nochild = false;
			while( !feof(fp) )
			{
				char buf[10240];
				if( !fgets(buf, sizeof(buf), fp) )
				{
					break;
				}

				if( nochild ) //����ʣ����CID
				{
					int spid;
					sscanf(buf, "ID=%d ", &spid);
					vecspex[spid - 1].CID = cid;
					vecthisc.push_back(vecspex[spid - 1]);

					continue;
				}

				//����ǰ2��
				++linecounter;
				if( linecounter < 3 )
				{
					continue;
				}

				if( buf[0] == '#' ) //������children/parents��Ϣ
				{
					//�ж�children/parents
					string strbuf(buf);
					char szparent[] = "Parents";
					int findpos = strbuf.find(szparent, 0);
					if( findpos != string::npos )
					{
						//��Parents
						continue;
					}

					//��<# Children: >���濪ʼ��ȡ�Ӵ��ļ�
					string thisline(buf + 12, strlen(buf) - 13);
					char buf2[10240];
					thisline.copy(buf2, thisline.length(), 0);
					buf2[thisline.length()] = '\0';

					//�Ӵؼ���vecchild
					char* p = buf2;
					char* d = " ";
					p = strtok(buf2, d);
					string childstr(p);
					if( p )
					{
						vecchild.push_back(childstr);
					}
					while( p )
					{
						p = strtok(NULL, d);
						if( !p )
						{
							break;
						}
						childstr = p;
						vecchild.push_back(childstr);
					}

					break;
				}
				else //���Ӵ�
				{
					//���ô��еĵ�CID
					++cid;

					int spid;
					sscanf(buf, "ID=%d ", &spid);
					vecspex[spid - 1].CID = cid;
					vecthisc.push_back(vecspex[spid - 1]);

					nochild = true;
				}
			}
			fclose(fp);
		}
		else
		{
			printf("�򿪴���Ϣ�ļ� %s ʧ��\n", filename.c_str());
			system("pause");
		}

		it = vecchild.erase(it);
		if( vecthisc.size() )
		{
			clusterout.push_back(vecthisc);
		}
	}
}

void ELKIProcess_OPTICS::FixCluster(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout)
{
	//ת��list
	vector<list<StopPointex>> listcout;
	for(int i = 0; i < clusterout.size(); ++i)
	{
		list<StopPointex> lc;
		for(int j = 0; j < clusterout[i].size(); ++j)
		{
			lc.push_back(clusterout[i][j]);
		}
		listcout.push_back(lc);
	}

	//���
	for(vector<list<StopPointex>>::iterator it = listcout.begin(); it != listcout.end(); ++it)
	{
		//��ÿ����
		StopPointex tspex;
		long long nearestid;

		//�����
		tspex = (*it).front();
		nearestid = GetNearestPointID(vecspex, tspex);
		if( nearestid != -1 )
		{
			long long newcid = vecspex[nearestid].CID;
			if( newcid != tspex.CID )
			{
				vecspex[tspex.ID].CID = newcid;
				(*it).erase((*it).begin());
				if( newcid != - 1 )
				{
					list<StopPointex>::iterator newpos = listcout[newcid].begin();
					++newpos;
					listcout[newcid].insert(newpos, vecspex[tspex.ID]);
				}
			}
		}

		//���β
		tspex = (*it).back();
		nearestid = GetNearestPointID(vecspex, tspex);
		if( nearestid != -1 )
		{
			long long newcid = vecspex[nearestid].CID;
			if( newcid != tspex.CID )
			{
				vecspex[tspex.ID].CID = newcid;
				list<StopPointex>::iterator itlast = (*it).end();
				--itlast;
				(*it).erase(itlast);
				if( newcid != - 1 )
				{
					list<StopPointex>::iterator newpos = listcout[newcid].begin();
					++newpos;
					listcout[newcid].insert(newpos, vecspex[tspex.ID]);
				}
			}
		}
	}

	//��������clusterout
	clusterout.clear();
	for(int i = 0; i < listcout.size(); ++i)
	{
		vector<StopPointex> tvec;
		for(list<StopPointex>::iterator it = listcout[i].begin(); it != listcout[i].end(); ++it)
		{
			tvec.push_back(*it);
		}
		clusterout.push_back(tvec);
	}
}

void ELKIProcess_DBSCAN::GenerateClusterAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout)
{
	wstring path = L"cluster_*.txt";
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(path.c_str(), &FindFileData);
    if( INVALID_HANDLE_VALUE == hFind )
    {
		printf("FindFirstFile()ʧ��,������� %d\n", GetLastError());
		system("pause");
	}

	wstring filename;
	int cid = 0;
	while( true )
    {
        if( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
        {
			filename = FindFileData.cFileName;
			FILE* fp = _wfopen(filename.c_str(), L"rt");
			if( fp )
			{
				vector<StopPointex> thisc;
				while( !feof(fp) )
				{
					char buf[128];
					if( !fgets(buf, sizeof(buf) - 1, fp) )
					{
						break;
					}

					if( buf[0] != '#' )
					{
						int id;
						sscanf(buf, "ID=%d ", &id);
						vecspex[id].CID = cid;
						thisc.push_back(vecspex[id]);
					}
				}

				fclose(fp);

				if( thisc.size() )
				{
					clusterout.push_back(thisc);
				}
			}
        }

        if( !FindNextFile(hFind, &FindFileData) )
		{
			break;
		}

		++cid;
    }
    FindClose(hFind);
}

void ELKIProcess_OPTICSOF::GenerateOPTICSOFAttr(vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout)
{
	FILE* fp = fopen("optics-outlier_order.txt", "rt");
	if( fp )
	{
		while( !feof(fp) )
		{
			char buf[128];
			if( !fgets(buf, sizeof(buf) - 1, fp) )
			{
				break;
			}

			int id;
			double lng, lat, opticsof;
			sscanf(buf, "ID=%d %lf %lf optics-outlier=%lf", &id, &lng, &lat, &opticsof);
			vecspex[id - 1].OPTICSOF = opticsof;
		}
		fclose(fp);

		for(int i = 0; i < clusterout.size(); ++i)
		{
			for(int j = 0; j < clusterout[i].size(); ++j)
			{
				clusterout[i][j].OPTICSOF = vecspex[clusterout[i][j].ID].OPTICSOF;
			}
		}
	}
}

void ELKIProcess_OPTICSOF::FixCluster(double ratio, vector<StopPointex>& vecspex, vector<vector<StopPointex>>& clusterout)
{
	//ת��list
	vector<list<StopPointex>> listcout;
	for(int i = 0; i < clusterout.size(); ++i)
	{
		list<StopPointex> lc;
		for(int j = 0; j < clusterout[i].size(); ++j)
		{
			lc.push_back(clusterout[i][j]);
		}
		listcout.push_back(lc);
	}

	//���
	for(vector<list<StopPointex>>::iterator it = listcout.begin(); it != listcout.end(); ++it)
	{
		//��ÿ���ذ�OPTICSOF��������
		(*it).sort(SortSPEXByOPTICSOF);
		list<StopPointex>::iterator it2 = (*it).begin();
		++it2;
		while( it2 != (*it).end() )
		{
			list<StopPointex>::iterator itpre = it2;
			--itpre;
			double thisopticsof = vecspex[(*it2).ID].OPTICSOF;
			double preopticsof = vecspex[(*itpre).ID].OPTICSOF;
			if( thisopticsof - preopticsof > ratio ) //ɾ��opticsof����ĵ�
			{
				while( it2 != (*it).end() )
				{
					vecspex[(*it2).ID].CID = -1;
					it2 = (*it).erase(it2);
				}

				break;
			}
			else
			{
				++it2;
			}
		}
	}

	//��������clusterout
	clusterout.clear();
	for(int i = 0; i < listcout.size(); ++i)
	{
		vector<StopPointex> tvec;
		for(list<StopPointex>::iterator it = listcout[i].begin(); it != listcout[i].end(); ++it)
		{
			tvec.push_back(*it);
		}
		clusterout.push_back(tvec);
	}
}

bool SortSPEXByOPTICSOF(const StopPointex& a, const StopPointex& b)
{
	return (a.OPTICSOF < b.OPTICSOF);
}