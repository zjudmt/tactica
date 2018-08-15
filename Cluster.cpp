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
	// initialize the disjoint set
	for (int i = 0; i <= cur_k; ++i)
		Set.push_back(-1);

	// initialize the vector of distances
	for (int i = 0; i < cur_k; i++)
		for (int j = 0; j < i; ++j)
		{
			double delta_d = getDeltaDistance(v_phase[i], v_phase[j]);
			dist d(delta_d, i + 1, j + 1);
			v_dist.push_back(d);
		}

	// clustering using the distance and the disjoint set
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

	// genertaing clusters and counting shot numbers using the results above
	vector<Cluster> v_cluster;
	cout << Set.size() << endl;
	for (int i = 1; i < Set.size(); ++i)
	{
		bool flag = insertPhase(v_cluster ,v_phase[i-1], Find(Set, i) );
		if(!flag){
			int root = Find(Set, i);
			Cluster c(root);
			v_cluster.push_back(c);
			flag = insertPhase(v_cluster ,v_phase[i-1], Find(Set, i) );
			if(!flag)
				cout << "Phase classification error at index: " << i << endl;
		}
	}
	cout << "Cluster over" << endl;

	// sort according to shot number
	sort(v_cluster.begin(), v_cluster.end());

	return v_cluster;
}

// insert a phase into the cluster it belongs
bool insertPhase(vector<Cluster> &v, Phase p, int r){
	for (std::vector<Cluster>::iterator i = v.begin(); i != v.end(); ++i){
		if(i->root == r){
			i->phases.push_back(p);
			//counting shots here
			if ( (p.end() - 1)->event_type >= 35 ){
				cout << "shot detected" << endl;
				i->shot ++;
			}
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
		cout << "root:" << i->root << " shot:" << i->shot << endl;
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


// write results to files with certain formats
void outputClusters(std::vector<Cluster> v, string team)
{
	for (std::vector<Cluster>::iterator i = v.begin(); i != v.end(); ++i)
	{
		int index = i - v.begin();
		string filepath = "Cluster\\" + team + "Cluster" + to_string(index) + ".txt";
		ofstream file(filepath);
		for (vector<Phase>::iterator j = i->phases.begin(); j != i->phases.end(); ++j){
			file << '[' ;
			for (std::vector<Event>::iterator ie = (*j).begin(); ie < (*j).end() - 1; ++ie)
			{
				file << ie->event_type << ",";
			}
			file << ((*j).end() - 1)->event_type;
			file << "]" << endl;
		}
		file.close();
	}
}