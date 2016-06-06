
#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <unordered_set>
#include <vector>

#include "MachineLearning/StudentInfo.h"
#include "Cluster/StudentCluster.h"
#include "Cluster/k_to_schools.h"
#include "Cluster/cluster.h"

using namespace std;
#define max_rec 7

StudentInfo *GetInputForOneStudent() {
    int _school;
    double _GPA;
    double _IELTS;
    int _TOEFL;
    double _GRE_overall;
    double _GRE_verbal;
    double _GRE_writing;
    int _research_intern;
    int _company_intern;
    int _paper;
    printf("Please enter the id of your current university: ");
    scanf("%d", &_school);
    printf("Please enter your GPA: ");
    scanf("%lf", &_GPA);
    printf("Please enter your IELTS score (enter -1 if you don't have IELTS score): ");
    scanf("%lf", &_IELTS);
    printf("Please enter your TOEFL score (enter -1 if not): ");
    scanf("%d", &_TOEFL);
    printf("Please enter your overall GRE score (enter -1 if not): ");
    scanf("%lf", &_GRE_overall);
    printf("Please enter your GRE verbal score (enter -1 if not): ");
    scanf("%lf", &_GRE_verbal);
    printf("Please enter your GRE writing score (enter -1 if not): ");
    scanf("%lf", &_GRE_writing);
    printf("Please enter the number of research projects you have participated: ");
    scanf("%d", &_research_intern);
    printf("Please enter the number of company internship: ");
    scanf("%d", &_company_intern);
    printf("Please enter the number of papers you have participated: ");
    scanf("%d", &_paper);
    return new StudentInfo(_school, _GPA, _IELTS, _TOEFL, _GRE_overall,
                           _GRE_verbal, _GRE_writing, _research_intern, _company_intern, _paper, -1);
}

void GetInputForKStudentsToSchools(const int& student_num,
                                   vector<SchoolScore>* school_score) {
    FILE *uni = fopen("A_University", "w+");
		FILE *map = fopen("A_mapping", "w+");
    int num_school;
    fscanf(uni, "%d", &num_school);
    for (int school = 0; school < num_school; ++school) {
        int id;
        double score;
        char name[100];
        int a, alias;
				fscanf(uni, "%d %lf", &id, &score);
        fgets(name, 100, uni);
				fscanf(map, "%d %d", &a, &alias);
        if (score == -1) continue;
        school_score->push_back(SchoolScore(score, alias, name, id));
    }
    fclose(uni);
    fclose(map);
		sort(school_score->begin(), school_score->end());
}

StudentCluster ConstructStudentClusterFromInput(vector<StudentInfo *> students_info) {
    int num_students, num_schools;
    
    freopen("MachineLearning/IOFiles/student_ids_for_school_tmp", "r", stdin);
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
    
    double **distance = (double**)calloc(num_students, sizeof(double*));
    for(int i = 0; i < num_students; ++i) {
				if (i == 0) {
					distance[i] = NULL;
					continue;
				}
        distance[i] = (double*)calloc(i, sizeof(double));
        for (int j = 0; j < i; ++j) {
            double similarity;
            bool valuable;
            StudentInfo::GetSimilarity(students_info[i], students_info[j], similarity, valuable);
            if (valuable) {
							distance[i][j] = fabs(similarity);
            } else {
							distance[i][j] = -1;
						}
        }
    }
    
    StudentCluster studentCluster(num_students, num_schools, distance, 
                                  student_ids_accepted_by_school);
    
    return studentCluster;
}

void OutputSchoolsForStudents(StudentCluster studentCluster) {
    int num_students = studentCluster.GetNumStudents();
    unordered_set<int> school_ids = 
				studentCluster.GetSchoolIdsForStudent(num_students - 1);
		
		int count = 0;
		vector<SchoolScore> school_score;
		GetInputForKStudentsToSchools(num_students, &school_score);	
		for (vector<SchoolScore>::iterator iter = school_score.begin();
				iter != school_score.end(); ++iter) {
			if (school_ids.find(iter->_id) != school_ids.end()) {
				cout << iter->_name << endl;
				if (iter->_alias != -1) {
					cout << iter->_alias << endl;
				} else {
					cout << "-1 " << iter->_name << endl;
				}
				++ count;
				if (count >= max_rec) break;
			}
		}	
}

int main(){
		vector<StudentInfo*> students_info;
    //School::ReadIn("MachineLearning/IOFiles/USASchool.in", "MachineLearning/IOFiles/ChinaSchool.in");
    School::ReadIn("A_University", "C_University");
		StudentInfo::ReadIn(students_info, "MachineLearning/IOFiles/StudentsInfo.in");
    StudentInfo::Standardize(students_info);
    StudentInfo *student_info = GetInputForOneStudent();
    StudentInfo::Standardize(student_info);
    students_info.push_back(student_info);
	
		StudentCluster studentCluster = ConstructStudentClusterFromInput(students_info);
		OutputSchoolsForStudents(studentCluster);

    return 0;
}
