#include "StudentInfo.h"
#include "BPNeuralNet.h"

#include <queue>

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
	FILE *fp;
    fp = fopen("IOFiles/k-nearest.out", "w");
	while (!pq.empty()) {
		fprintf(fp, "%d ", pq.top().student_id);
		pq.pop();	
	}
	fclose(fp);
	return 0;
	
}