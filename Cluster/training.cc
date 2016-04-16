#include "cluster.h"
#include <vector>
#include <limits.h>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

const int knpass = 10;

class Cluster {
	public:
		Cluster();
		~Cluster();

		void SetInputs();
		
		// Calc cost function for school 'schoolid' within cluster results 'clusterid'.
		int CalcCostFunction(int schoolid, int *clusterid);

		// Run kmedois n times and calculate optimal K for each school.
		void CalcClusterResult();
		
		// Calc all students that school 'schoolid' might accept, store the results in Choice. 
		void CalcSelectedStudents(int schoolid);
		
		void Output();
	
		// Get potential schools for each student.
		void Run();
	
	private:
		// Students number.
		int nelements;
		// Schools number.
		int nschools;
		// Nelements times cluster results.
		vector<int *> cluster_result;
		// Distance matrix between all students.
		double **distance;
		// Optimal K value in K-medoids algorithm for each school.
		vector<int> Kvalue;
		// For each school, the id of students that they accepted.
		vector<vector<int> > schools;
		// For each student, Choice stores all schools that might accpet him.
		vector<unordered_set<int> > choice;
};

Cluster::Cluster() {
	nelements = 0;
	nschools = 0;
	cluster_result.clear();
	distance = NULL;
	Kvalue.clear();
	schools.clear();
	choice.clear();
}

Cluster::~Cluster() {
	free(distance);
	while (!cluster_result.empty()) {
		free(cluster_result.back());
		cluster_result.pop_back();
	}
}

void Cluster::SetInputs() {
	// Read from input.
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
		schools.push_back(students);
	}
	
	// Initialize other member variables.
	unordered_set<int> tmp_set;
	tmp_set.clear();
	for (int i = 0; i < nelements; ++i) {
		choice.push_back(tmp_set);
	}
	for (int i = 0; i < nschools; ++i) {
		Kvalue.push_back(0);
	}
}

int Cluster::CalcCostFunction(int schoolid, int *clusterid) {
	int Ret = 1;
	for (int i = 0; i < schools[schoolid].size(); ++i) {
		for (int j = i+1; j < schools[schoolid].size(); ++j) {
			Ret += (clusterid[i] != clusterid[j]);
		}
	}
	return Ret;
}

void Cluster::CalcClusterResult() {
	double error;
	int ifound;
	int *clusterid;
	vector<double> min_cost(nschools, (double)INT_MAX);

	for (int i = 1; i <= nelements; ++i) {
		clusterid = (int*)calloc(nelements, sizeof(int));
		kmedoids(i, nelements, distance, knpass, clusterid, &error, &ifound);
		cluster_result.push_back(clusterid);
	
		error = max(1, log(max(error, 1)));
		for (int j = 0; j < nschools; ++j) {
			double cost = CalcCostFunction(j, clusterid) * error;
			if (cost < min_cost[j]) {
				min_cost[j] = cost;
				Kvalue[j] = i-1;
			}
		}
	}
}

void Cluster::CalcSelectedStudents(int schoolid) {
	// Sum the amount of accepted students in each cluster.
	unordered_map<int, int> count_cluster;
	for (int i = 0; i < schools[schoolid].size(); ++i) {
		count_cluster[cluster_result[Kvalue[schoolid]][schools[schoolid][i]]] ++;
	}
	
	// Find the maximum of accepted students in one cluster.
	int max_cluster = 0;
	for (unordered_map<int, int>::iterator iter = count_cluster.begin(); iter != count_cluster.end(); ++iter) {
		max_cluster = max(max_cluster, iter->second);
	}
	
	// Calc the cluster ids which is acceptible for school 'schoolid'.
	max_cluster = (int) (max_cluster * 0.5 + 0.5);
	unordered_set<int> accepted_cluster;
	for (unordered_map<int, int>::iterator iter = count_cluster.begin(); iter != count_cluster.end(); ++iter) {
		if (iter->second >= max_cluster) {
			accepted_cluster.insert(iter->first);
		}
	}
	
	// Update choice.
	for (int i = 0; i < nelements; ++i) {
		if (accepted_cluster.find(cluster_result[Kvalue[schoolid]][i]) != accepted_cluster.end()) {
			choice[i].insert(schoolid);
		}
	}
}

void Cluster::Output() {
	for (int i = 0; i < nelements; ++i) {
		printf("%lu ", choice[i].size());
		for (unordered_set<int>::iterator iter = choice[i].begin(); iter != choice[i].end(); ++iter) {
			printf("%d ", *iter);
		}
		printf("\n");
	}
}

void Cluster::Run() {
	SetInputs();
	CalcClusterResult();

	for (int i = 0; i < nschools; ++i) {
		CalcSelectedStudents(i);		
	}
	Output();	
}

int main() {
	Cluster cluster;
	cluster.Run();
	return 0;
}
