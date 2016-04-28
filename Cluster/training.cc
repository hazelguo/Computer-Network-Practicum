#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <unordered_set>
#include <vector>

#include "StudentCluster.h"

using namespace std;

// Local methods for main.
#define cube(p) ((p)*(p)*(p))
StudentCluster ConstructStudentClusterFromInput() {
	int num_students, num_schools;

	cerr << "Reading....0" << endl;

	freopen("../MachineLearning/IOFiles/student_ids_for_school_tmp", "r", stdin);
	scanf("%d%d", &num_students, &num_schools);
	vector<vector<int> > student_ids_accepted_by_school;
	for (int i = 0; i < num_schools; ++i) {
		vector<int> students;
		students.clear();
		int j, k;
		for (scanf("%d", &j); j ; --j) {
			scanf("%d", &k);
			students.push_back(k);
		}
		student_ids_accepted_by_school.push_back(students);	
	}
	fclose(stdin);

	cerr << "Reading.....1" << endl;
	
	//freopen("../MachineLearning/IOFiles/StudentSimi*", "r", stdin);
	freopen("../MachineLearning/IOFiles/OriginalSimilarityMatrix", "r", stdin);
	double **distance = (double**)calloc(num_students, sizeof(double*));
	double *tmp = (double*)calloc(num_students, sizeof(double));
	for (int i = 0; i < num_students; ++i) {
		for (int j = 0; j < num_students; ++j) {
			scanf("%lf", &tmp[j]);
		}
		if (i == 0) {
			distance[i] = NULL;
			continue;
		}
		distance[i] = (double*)calloc(i, sizeof(double));
		for (int j = 0; j < i; ++j) {
			distance[i][j] = fabs(tmp[j]);
			//scanf("%lf", &distance[i][j]);
//			printf("%lf ", distance[i][j]);
		}
		printf("\n");
	}
	fclose(stdin);
	
	cerr << "Reading.....2" << endl;
	
	StudentCluster studentCluster(num_students, num_schools, distance, 
			student_ids_accepted_by_school);
	
	return studentCluster;
}

void OutputSchoolsForStudents(StudentCluster studentCluster) {
//	cerr << "Output.....-1" << endl;

	freopen("school_ids_for_student", "w", stdout);

	int num_students = studentCluster.GetNumStudents();
	for (int student_id = 0; student_id < num_students; ++student_id) {
//		cerr << "Output....." << student_id << endl;
		unordered_set<int> school_ids = 
				studentCluster.GetSchoolIdsForStudent(student_id);
		printf("%lu ", school_ids.size());
		for (unordered_set<int>::iterator iter = school_ids.begin(); 
				iter != school_ids.end(); ++iter) {
			printf("%d ", *iter);
		}
		printf("\n");
	}
}

int main() {
	StudentCluster studentCluster = ConstructStudentClusterFromInput();
	OutputSchoolsForStudents(studentCluster);
	return 0;
}
