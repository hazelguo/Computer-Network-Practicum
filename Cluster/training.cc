#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <unordered_set>
#include <vector>

#include "StudentCluster.h"

using namespace std;

// Local methods for main.

StudentCluster ConstructStudentClusterFromInput() {
	int num_students, num_schools;

	freopen("../MachineLearning/IOFiles/student_ids_for_school", "r", stdin);
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
	
	freopen("../MachineLearning/IOFiles/StudentSimilarityMatrix", "r", stdin);
	double **distance = (double**)calloc(num_students, sizeof(double*));
	double *tmp = (double*)calloc(num_students, sizeof(double));
	for (int i = 0; i < num_students; ++i) {
		distance[i] = (double*)calloc(i, sizeof(double));
		for (int j = 0; j < num_students; ++i) {
			scanf("%lf", tmp[j]);
		}
		for (int j = 0; j < i; ++j) {
			distance[i][j] = tmp[j];
			//scanf("%lf", &distance[i][j]);
		}
	}
	delete [] tmp;
	fclose(stdin);
	StudentCluster studentCluster(num_students, num_schools, distance, 
			student_ids_accepted_by_school);
	return studentCluster;
}

void OutputSchoolsForStudents(StudentCluster studentCluster) {
	freopen("school_ids_for_student", "w", stdou);

	int num_students = studentCluster.GetNumStudents();
	for (int student_id = 0; student_id < num_students; ++student_id) {
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
