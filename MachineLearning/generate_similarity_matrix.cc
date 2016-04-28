#include "StudentInfo.h"
#include "BPNeuralNet.h"

using namespace std;

BPNeuralNet BP_neural_net;

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

double calculate_similarity(StudentInfo* a, StudentInfo* b){
    vector<double> properties;
    vector<double> similarity(1);
    StudentInfo::GetProperties(a, b, properties);
    BP_neural_net.recognize(properties, similarity);
    return similarity[0];
}

int main(){

  /*  vector<StudentInfo*> students_info;
    BP_neural_net.GetWeightAndThreshold();
    School::ReadIn("IOFiles/USASchool.in", "IOFiles/ChinaSchool.in");
    StudentInfo::ReadIn(students_info, "IOFiles/StudentsInfo.in");
    StudentInfo::Standardize(students_info);
    int student_num = students_info.size();
    FILE *fp;
    fp = fopen("IOFiles/StudentSimilarityMatrix", "w");
    for(int i = 0; i < student_num; ++i) {
        for (int j = 0; j < student_num; ++j) {
            double similarity = calculate_similarity(students_info[i], students_info[j]);
            fprintf(fp, "%lf ", similarity);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
*/
    vector<StudentInfo*> students_info;
//    StudentSimilarity tmp;
//    priority_queue<StudentSimilarity> pq;
//    BP_neural_net.GetWeightAndThreshold();
    School::ReadIn("IOFiles/USASchool.in", "IOFiles/ChinaSchool.in");
    cerr << "What hapensss.s.....3" << endl;
		StudentInfo::ReadIn(students_info, "IOFiles/StudentsInfo.in");
		cerr << "What happensss.....0" << endl;
    StudentInfo::Standardize(students_info);
		cerr << "What happenss .........." << endl; 
    StudentInfo *student_info = GetInputForOneStudent();
    //input_info.push_back(student_info);
    //GetInputForOneStudent(student_info);
		cerr << "what happensss......1" << endl;
    StudentInfo::Standardize(student_info);
    students_info.push_back(student_info);
		
		int student_num = students_info.size(); 
    FILE *fp = fopen("IOFiles/OriginalSimilarityMatrix", "w");
    for(int i = 0; i < student_num; ++i) {
        for (int j = 0; j < student_num; ++j) {
            double similarity;
            bool valuable;
            StudentInfo::GetSimilarity(students_info[i], students_info[j], similarity, valuable);
            if (valuable) fprintf(fp, "%lf ", similarity);
            else fprintf(fp, "NULL ");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);


    return 0;
    
}
