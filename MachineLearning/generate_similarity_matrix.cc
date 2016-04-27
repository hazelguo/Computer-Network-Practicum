#include "StudentInfo.h"
#include "BPNeuralNet.h"

using namespace std;

BPNeuralNet BP_neural_net;

double calculate_similarity(StudentInfo* a, StudentInfo* b){
    vector<double> properties;
    vector<double> similarity(1);
    StudentInfo::GetProperties(a, b, properties);
    BP_neural_net.recognize(properties, similarity);
    return similarity[0];
}

int main(){

    vector<StudentInfo*> students_info;
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

    fp = fopen("IOFiles/OriginalSimilarityMatrix", "w");
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