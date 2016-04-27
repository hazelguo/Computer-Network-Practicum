#include "StudentInfo.h"
#include "BPNeuralNet.h"

#include <vector>
#include <iostream>

#define PROPERTY_NUM 8

using namespace std;

vector<StudentInfo*> students_info;
int student_num;

int main(){

    School::ReadIn("IOFiles/USASchool.in", "IOFiles/ChinaSchool.in");
	StudentInfo::ReadIn(students_info, "IOFiles/StudentsInfo.in");
    StudentInfo::Standardize(students_info);
/*
    for (int i = 0; i < 100; i++){
        for (int j = 0; j < 10; j++){
            printf("%lf ", students_info[i]->attributes[j]);
        }
        printf("\n");
    }

    
    cout << School::C_school.size() << endl;
    for (int i = 0; i < School::C_school.size(); ++i){
        cout << School::C_school[i]->rank << School::C_school[i]->name << endl;
    }
   */ 
	student_num = students_info.size();
	
	BPNeuralNet BP_neural_net;
	int times=0;
    vector<vector<double> > X;
    vector<vector<double> > Y;
    for (int i = 0; i < student_num; ++i) {
        for (int j = 0; j < student_num; ++j) {
            bool flag;
            double similarity;
            StudentInfo::GetSimilarity(students_info[i], students_info[j],
                                       similarity, flag);
            if (flag) {
                vector<double> properties;
                vector<double> tmp;
                StudentInfo::GetProperties(students_info[i], students_info[j],
                                           properties);
                X.push_back(properties);
                tmp.clear();
                tmp.push_back(abs(similarity));
                Y.push_back(tmp);
            }
        }
    }
    cout << X.size() << endl;
    BP_neural_net.trainsample = X.size();
    double last_error = 0.0;
    while(abs(last_error - BP_neural_net.error) > 0.00001)  
    {   
        last_error = BP_neural_net.error;
        BP_neural_net.e=0.0;  
        times++;  
        BP_neural_net.train(X, Y);
        cout<<"Times="<<times<<" error="<<BP_neural_net.error<<endl;
    }  
    cout<<"trainning complete..."<<endl;
    BP_neural_net.SaveWeightAndThreshold();
    
/*    double m[innode]={1,1,1};  
    double *r=BP_neural_net.recognize(m);  
    for(int i=0;i<outnode;++i)  
       cout<<BP_neural_net.result[i]<<" ";  
    double cha[trainsample][outnode];  
    double mi=100;  
    double index;  
    for(int i=0;i<trainsample;i++)  
    {  
        for(int j=0;j<outnode;j++)  
        {  
            //找差值最小的那个样本  
            cha[i][j]=(double)(fabs(Y[i][j]-BP_neural_net.result[j]));  
            if(cha[i][j]<mi)  
            {  
                mi=cha[i][j];  
                index=i;  
            }  
        }  
    }  
    for(int i=0;i<innode;++i)  
       cout<<m[i];  
    cout<<" is "<<index<<endl;  
    cout<<endl;  */

    return 0; 
}