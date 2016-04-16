#include "cluster.h"

int main() {
	double **distance;
	int *clusterid;
	double error;
	int ifound;

	kmedoids(0, 0, distance, 0, clusterid, &error, &ifound);
	return 0;  
}
