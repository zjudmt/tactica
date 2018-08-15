#ifndef __CLUSTER_H__
#define __CLUSTER_H__

#include "tactica.h"
using namespace std;

class dist
{
public:
	
	double distance;
	int i1;
	int i2;

	dist(double _distance, int _i1, int _i2){
		distance = _distance;
		i1 = _i1;
		i2 = _i2;
	}
	bool operator <(const dist &d) const
	{
		return distance < d.distance;
	}
};

class Cluster
{
public:
	Cluster(int _root){
		root = _root;
		shot = 0;
		// cout << "Cluster(): " << root << endl;
	}
	vector<Phase> phases;
	int root;
	int shot;

	bool operator<(const Cluster &c) const
	{
		return this->shot < c.shot;
	}
};

bool insertPhase(vector<Cluster> &v, Phase p, int r);
vector<Cluster> AHCluster(vector<Phase> v_phase, int k);
int Find(vector<int> &a, int index);
void Union(vector<int> &a, int c1, int c2);
void printClusters(std::vector<Cluster> v);
void outputClusters(std::vector<Cluster> v, string team);

#endif
