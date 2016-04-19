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
    StudentInfo::ReadIn(students_info, "IOFiles/StudentsInfo.in");
    School::ReadIn("IOFiles/ChinaSchool.in", "IOFiles/USASchool.in");
    int student_num = students_info.size();
    FILE *fp;
    fp = fopen("IOFiles/StudentSimilarityMatrix.in", "w");
    for(int i = 0; i < student_num; ++i) {
        for (int j = 0; j < student_num; ++j) {
            double similarity = calculate_similarity(students_info[i], students_info[j]);
            fprintf(fp, "%lf ", similarity);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
    
}