#include "StudentInfo.h"
#include "BPNeuralNet.h"
#include "../Cluster/k_to_schools.h"

#include <queue>
#include <vector>
#include <unordered_set>

const int k = 3;

using namespace std;

BPNeuralNet BP_neural_net;

class StudentSimilarity{
public:
	double similarity;
	int student_id;
	bool operator<(const StudentSimilarity &a) const {
		return similarity > a.similarity;
	}
};

double calculate_similarity(StudentInfo* a, StudentInfo* b){
	vector<double> properties;
	vector<double> similarity(1);
	StudentInfo::GetProperties(a, b, properties);
	BP_neural_net.recognize(properties, similarity);
	return similarity[0];
}

void GetInputForKStudentsToSchools(const int& student_num,
		vector<unordered_set<int> >* school_ids_for_student, 
		vector<SchoolScore>* school_score) {
	freopen("../Cluster/school_ids_for_student", "r", stdin);
	unordered_set<int> school_ids;
	for (int student = 0; student < student_num; ++student) {
		int num_school;
		scanf("%d", &num_school);
		school_ids.clear();
		for (int school = 0; school < num_school; ++school) {
			int p;
			scanf("%d", &p);
			school_ids.insert(p);
		}
		school_ids_for_student->push_back(school_ids);
	}
	fclose(stdin);

	freopen("../A_University", "r", stdin);
	int num_school;
	scanf("%d", &num_school);
	for (int school = 0; school < num_school; ++school) {
		int id;
		double score;
		char name[100];
		scanf("%d %lf", &id, &score);
		gets(name);
		school_score->push_back(SchoolScore(score, name, id));
	} 
	fclose(stdin);
	sort(school_score->begin(), school_score->end());
}

void GetInputForOneStudent(StudentInfo *student_info) {
	printf("Please enter the id of your current university: ");
	scanf("%d", &student_info->school);
	printf("Please enter your GPA: ");
	scanf("%lf", &student_info->GPA);
	printf("Please enter your IELTS score (enter -1 if you don't have IELTS score): ");
	scanf("%lf", &student_info->IELTS);
	printf("Please enter your TOEFL score (enter -1 if not): ");
	scanf("%d", &student_info->TOEFL);
	printf("Please enter your overall GRE score (enter -1 if not): ");
	scanf("%lf", &student_info->GRE_overall);
	printf("Please enter your GRE verbal score (enter -1 if not): ");
	scanf("%lf", &student_info->GRE_verbal);
	printf("Please enter your GRE writing score (enter -1 if not): ");
	scanf("%lf", &student_info->GRE_writing);
	printf("Please enter the number of research projects you have participated: ");
	scanf("%d", &student_info->research_intern);
	printf("Please enter the number of company internship: ");
	scanf("%d", &student_info->company_intern);
	printf("Please enter the number of papers you have participated: ");
	scanf("%d", &student_info->paper);
}

int main(){

	StudentInfo *student_info;
	vector<StudentInfo*> students_info;
	StudentSimilarity tmp;
	priority_queue<StudentSimilarity> pq;
    BP_neural_net.GetWeightAndThreshold();
    School::ReadIn("IOFiles/USASchool.in", "IOFiles/ChinaSchool.in");
    StudentInfo::ReadIn(students_info, "IOFiles/StudentsInfo.in");
//    GetInputForOneStudent(student_info);
		StudentInfo::Standardize(students_info);
	
	//By gzh: generate accepted student ids for each school.
	freopen("IOFiles/student_ids_for_school", "w", stdout);
	vector<vector<int> > student_ids;
	vector<int> ttmp;
	for (int i = 0; i < 100; ++i) {
		student_ids.push_back(ttmp);
	}
	for (int i = 0; i < students_info.size(); ++i) {
		for (vector<Offer>::iterator one_offer = students_info[i]->offers.begin();
				one_offer != students_info[i]->offers.end(); ++one_offer) {
			student_ids[one_offer->school].push_back(i);
		}
	}	
	while (student_ids.back().size() == 0) {
		student_ids.pop_back();
	}
	printf("%ld %ld\n", students_info.size(), student_ids.size());
	for (vector<vector<int> >::iterator iter = student_ids.begin();
			iter != student_ids.end(); ++iter) {
		printf("%ld ", iter->size());
		for (vector<int>::iterator one_school = iter->begin(); 
				one_school != iter->end(); ++one_school) {
			printf("%d ", *one_school);
		}
		printf("\n");
	}
	fclose(stdout);
	return 0;

	int student_num = students_info.size();
	for(int i = 0; i < student_num; ++i){
		tmp.similarity = calculate_similarity(students_info[i], student_info);
		tmp.student_id = i;
		if (pq.size() < k) pq.push(tmp);
		else{
			if (tmp < pq.top()){
				pq.pop();
				pq.push(tmp);
			}
		}
	}
/*
	FILE *fp;
    fp = fopen("IOFiles/k-nearest.out", "w");
	while (!pq.empty()) {
		fprintf(fp, "%d ", pq.top().student_id);
		pq.pop();	
	}
	fclose(fp);
*/

	vector<unordered_set<int> > school_ids_for_student;
	vector<SchoolScore> school_score;
	GetInputForKStudentsToSchools(student_num, &school_ids_for_student, 
			&school_score);
	KStudentsToSchools *k_students_to_schools = 
			new KStudentsToSchools(school_ids_for_student, school_score);

	vector<int> k_students;
	while (!pq.empty()) {
		k_students.push_back(pq.top().student_id);
		pq.pop();
	}
	vector<string> school_recommendation;
	k_students_to_schools->SchoolRecommendation(k_students, &school_recommendation);
		
	return 0;
	
}
