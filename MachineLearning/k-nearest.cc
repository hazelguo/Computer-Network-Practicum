#include "StudentInfo.h"
#include "BPNeuralNet.h"
#include "../Cluster/k_to_schools.h"

#include <queue>
#include <vector>
#include <unordered_set>
#include <set>

const int k = 2;
const int max_rec = 7;

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
	//freopen("IOFiles/school_ids_for_student", "r", stdin);
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
		if (score == -1) continue;
		school_score->push_back(SchoolScore(score, name, id));
	} 
	fclose(stdin);
	sort(school_score->begin(), school_score->end());
//	for (vector<SchoolScore>::iterator iter = school_score->begin();
//			iter != school_score->end(); ++iter) {
//		cout << iter->_score << " " << iter->_id << " " << iter->_name << endl;
//	}
}

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
#define cube(p) ((p)*(p)*(p))
#define sqr(p) ((p)*(p))
int main(){

	//StudentInfo *student_info;
	vector<StudentInfo *> input_info;
	vector<StudentInfo*> students_info;
	StudentSimilarity tmp;
	priority_queue<StudentSimilarity> pq;
    BP_neural_net.GetWeightAndThreshold();
    School::ReadIn("IOFiles/USASchool.in", "IOFiles/ChinaSchool.in");
    StudentInfo::ReadIn(students_info, "IOFiles/StudentsInfo.in");
	StudentInfo::Standardize(students_info);
    
  StudentInfo *student_info = GetInputForOneStudent();
	//input_info.push_back(student_info);  
	//GetInputForOneStudent(student_info);
	StudentInfo::Standardize(student_info);    
    
	cerr << student_info->GPA << " " << student_info->school << " " << student_info->GRE_overall << " " << student_info->GRE_verbal << endl;	
	cerr << students_info[0]->GPA << " " << students_info[0]->school << " " << students_info[0]->GRE_overall << " " << students_info[0]->GRE_verbal << endl;	
	
	//By gzh: generate accepted student ids for each school.
	/*freopen("IOFiles/RejectInfo", "r", stdin);
	int num_rej;
	set<int> rej;
	scanf("%d", &num_rej);
	for (int i = 0; i < num_rej; ++i) {
		int a;
		scanf("%d", &a);
		rej.insert(a);
	}
	fclose(stdin);

	freopen("IOFiles/student_ids_for_school_tmp", "w", stdout);
	vector<vector<int> > student_ids;
	vector<int> ttmp;
	for (int i = 0; i < 100; ++i) {
		student_ids.push_back(ttmp);
	}
	for (int i = 0; i < students_info.size(); ++i) {
		if (rej.find(i) != rej.end()) continue;
		for (vector<Offer>::iterator one_offer = students_info[i]->offers.begin();
				one_offer != students_info[i]->offers.end(); ++one_offer) {
			student_ids[one_offer->school].push_back(i);
			printf("%d %d\n", i, one_offer->school);
		}
	}	
	while (student_ids.back().size() == 0) {
		student_ids.pop_back();
	}*/
/*	printf("%ld %ld\n", students_info.size(), student_ids.size());
	for (vector<vector<int> >::iterator iter = student_ids.begin();
			iter != student_ids.end(); ++iter) {
		printf("%ld ", iter->size());
		for (vector<int>::iterator one_school = iter->begin(); 
				one_school != iter->end(); ++one_school) {
			printf("%d ", *one_school);
		}
		printf("\n");
	}
	fclose(stdout);*/
/*	freopen("IOFiles/school_ids_for_student", "w", stdout);
	vector<vector<int> > student_ids;
	vector<int> ttmp;
	for (int i = 0; i < 100; ++i) {
		student_ids.push_back(ttmp);
	}
	for (int i = 0; i < students_info.size(); ++i) {
		if (rej.find(i) != rej.end()) {
			printf("0\n");
			continue;
		}
		printf("%lu ", students_info[i]->offers.size());
		for (vector<Offer>::iterator one_offer = students_info[i]->offers.begin();
				one_offer != students_info[i]->offers.end(); ++one_offer) {
			printf("%d ", one_offer->school);
		}
		printf("\n");
	}	
	fclose(stdout);
	return 0;*/

	int student_num = students_info.size();
	for(int i = 0; i < student_num; ++i){
		tmp.similarity = calculate_similarity(students_info[i], student_info);
		tmp.similarity = cube(tmp.similarity*100);
		//double simi;
		//bool valuable;
		//StudentInfo::GetSimilarity(students_info[i], student_info, simi, valuable);
		//tmp.similarity = fabs(simi); 
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
/*	while (!pq.empty()) {
		printf("%d ", pq.top().student_id);
		pq.pop();	
	}*/
//	return 0;

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
	int num_rec = school_recommendation.size();
	if (num_rec > max_rec) num_rec = max_rec;
	for (int i = 0; i < num_rec; ++i) {
		cout << school_recommendation[i] << endl;
	}
	while (!pq.empty()) {
		printf("%d ", pq.top().student_id);
		pq.pop();	
	}
	return 0;
	
}
