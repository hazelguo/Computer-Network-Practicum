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

int main(){

	StudentInfo *student_info;
	vector<StudentInfo*> students_info;
	StudentSimilarity tmp;
	priority_queue<StudentSimilarity> pq;
	BP_neural_net.GetWeightAndThreshold();
	StudentInfo::ReadIn(students_info, "IOFiles/StudentsInfo.in");
	School::ReadIn("IOFiles/ChinaSchool.in", "IOFiles/USASchool.in");
	student_info = StudentInfo::ReadInOne("IOFiles/StudentInfo.in");
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
