#include "cluster.h"
#include <vector>
#include <limits.h>
#include <cstdlib>

using namespace std;

const int knpass = 1;

class Cluster {
	public:
		Cluster();
		~Cluster();

		void SetInputs();
		
		int CalcCostFunction(int schoolid, int *clusterid);

		// Run kmedois n times and calculate optimal K for each school.
		void CalcClusterResult();
		
		// Get potential schools for each student.
		void Run();
	
	private:
		int nelements;
		int nschools;
		vector<int *> cluster_result;
		double **distance;
		vector<int> Kvalue;
		vector<vector<int> > Schools;
};

Cluster::Cluster() {
	nelements = 0;
	nschools = 0;
	cluster_result.clear();
	distance = NULL;
	Schools.clear();
}

Cluster::~Cluster() {
	free(distance);
	while (!cluster_result.empty()) {
		free(cluster_result.back());
		cluster_result.pop_back();
	}
}

void Cluster::SetInputs() {
	scanf("%d%d", &nelements, &nschools);
	
	distance = (double**)calloc(nelements, sizeof(double*));
	for (int i = 0; i < nelements; ++i) {
		distance[i] = (double*)calloc(i, sizeof(double));
		for (int j = 0; j < i; ++j) {
			scanf("%lf", &distance[i][j]);
		}
	}	
	
	for (int i = 0; i < nschools; ++i) {
		vector<int> students;
		students.clear();
		int j, k;
		for (scanf("%d", &j); j ; --j) {
			scanf("%d", &k);
			students.push_back(k);
		}
		Schools.push_back(students);
		Kvalue.push_back(0);
	}	
}

int Cluster::CalcCostFunction(int schoolid, int *clusterid) {
	int Ret = 0;
	for (int i = 0; i < Schools[schoolid].size(); ++i) {
		for (int j = 0; j < Schools[schoolid].size(); ++j) {
			Ret += (clusterid[i] != clusterid[j]);
		}
	}
	return Ret;
}

void Cluster::CalcClusterResult() {
	double error;
	int ifound;
	int *clusterid;
	vector<int> min_cost(nschools, INT_MAX);

	for (int i = 1; i <= nelements; ++i) {
		clusterid = (int*)calloc(nelements, sizeof(int));
		kmedoids (i, nelements, distance, knpass, clusterid, &error, &ifound);
		cluster_result.push_back(clusterid);
		
		for (int j = 0; j < nschools; ++j) {
			int p = CalcCostFunction(j, clusterid);
			if (p < min_cost[j]) {
				min_cost[j] = p;
				Kvalue[j] = i;
			}
		}
	}
}

int main() {
	Cluster cluster;
	cluster.Run();
	return 0;
}
