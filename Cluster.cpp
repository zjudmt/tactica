#include <fstream>
#include "Cluster.h"

using namespace std;


int Find(vector<int> &a, int index)
{
	int ret;
	if( a[index] < 0)
		ret = index;
	else
		ret = Find(a, a[index]);

	return ret;
}

void Union(vector<int> &a, int c1, int c2)
{
	if( a[c1] < a[c2] ){
		a[c1] += a[c2];
		a[c2] = c1;
	}
	else{
		a[c2] += a[c1];
		a[c1] = c2;
	}
}

// 聚类函数，输入phase的向量以及聚类目标数k，返回聚类结果的并查集
std::vector<Cluster> AHCluster(vector<Phase> v_phase, int k){
	vector<int> Set;
	vector<dist> v_dist;
	int cur_k = v_phase.size();
	for (int i = 0; i <= cur_k; ++i)
		Set.push_back(-1);

	for (int i = 0; i < cur_k; i++)
		for (int j = 0; j < i; ++j)
		{
			double delta_d = getDeltaDistance(v_phase[i], v_phase[j]);
			dist d(delta_d, i + 1, j + 1);
			v_dist.push_back(d);
		}

	sort(v_dist.begin(), v_dist.end());
	for (std::vector<dist>::iterator ptr = v_dist.begin(); ptr != v_dist.end(); ++ptr)
	{
		if(cur_k <= k)
			break;
		if( Find(Set,  ptr->i1) == Find(Set,  ptr->i2) )
			continue;
		else{
			Union(Set, Find(Set, ptr->i1), Find(Set, ptr->i2));
			cur_k --;
		}
	}

	vector<Cluster> v_cluster;
	cout << Set.size() << endl;
	for (int i = 1; i < Set.size(); ++i)
	{
		bool flag = insertPhase(v_cluster ,v_phase[i-1], Find(Set, i) );
		// cout << i << " " << flag << endl;
		if(!flag){
			int root = Find(Set, i);
			Cluster c(root);
			v_cluster.push_back(c);
			flag = insertPhase(v_cluster ,v_phase[i-1], Find(Set, i) );
			if(!flag)
				cout << "Phase classification error at index: " << i << endl;
		}
	}
	cout << "Cluster end" << endl;

	return v_cluster;
}

bool insertPhase(vector<Cluster> &v, Phase p, int r){
	for (std::vector<Cluster>::iterator i = v.begin(); i != v.end(); ++i){
		if(i->root == r){
			i->phases.push_back(p);
			return true;
		}
	}

	return false;
}

void printClusters(std::vector<Cluster> v)
{
	cout << "Cluster Number: " << v.size() << endl;
	for (std::vector<Cluster>::iterator i = v.begin(); i != v.end(); ++i)
	{
		cout << i->root << endl;
		for (vector<Phase>::iterator j = i->phases.begin(); j != i->phases.end(); ++j){
			for (std::vector<Event>::iterator ie = (*j).begin(); ie != (*j).end(); ++ie)
			{
				cout << ie->team << " " << ie->event_type << " |";
			}
			cout << endl;
		}
		cout << "#########" << endl;
	}
}

void outputClusters(std::vector<Cluster> v, string team)
{
	for (std::vector<Cluster>::iterator i = v.begin(); i != v.end(); ++i)
	{
		int index = i - v.begin();
		string filepath = "Cluster\\" + team + "Cluster" + to_string(index) + ".txt";
		cout << filepath << endl;
		ofstream file(filepath);
		for (vector<Phase>::iterator j = i->phases.begin(); j != i->phases.end(); ++j){
			file << '[' ;
			for (std::vector<Event>::iterator ie = (*j).begin(); ie < (*j).end() - 1; ++ie)
			{
				// file << ie->team << " " << ie->event_type << " |";
				file << ie->event_type << ",";
			}
			file << ((*j).end() - 1)->event_type;
			file << "]" << endl;
		}
		file.close();
	}
}