#ifndef STUDENT_INFO_H
#define STUDENT_INFO_H

#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

class School {
public:
    /*static vector<double> china_school;
    static vector<double> USA_school;
    static void ReadIn(const char *filepath_china, const char *filepath_USA);
    static double GetChinaSchoolRank(int id);
    static double GetUSASchoolRank(int id);*/
    double rank;
    string name;
    School(double _rank, string _name);
    static void ReadIn(vector<School*> &schools, const char *filepath);
    static void ReadIn(const char *filepath_A, const char *filepath_C);
    static vector<School*> A_school;
    static vector<School*> C_school;
};

class Offer {
public:
    int school;
    int degree;
    // "0" represents Master
    // "1" represents PhD
    bool operator<(const Offer &a) const;
    bool operator==(const Offer &a) const;
    Offer(int _school, int _degree);
};

class StudentInfo {
    public:
        int school;
        double GPA;
        double IELTS;
        int TOEFL;
        double GRE_overall;
        double GRE_verbal;
        double GRE_writing;
        int research_intern;
        int company_intern;
        int paper;
        int final_decision;
        vector<Offer> offers;
        vector<double> attributes;
        static int attributes_num;
        static vector<double> total;
        static vector<int> count;
        static vector<double> average;
        static vector<double> std;
        StudentInfo(int _school, double _GPA, double _IELTS, int _TOEFL,
                    double _GRE_overall, double _GRE_verbal, double _GRE_writing,
                    int _research_intern, int _company_intern,
                    int _paper, int _final_decision);
        //void CalculateAverage();
        //void Normalize();
        static void Standardize(vector<StudentInfo*> &students_info);
        static void Standardize(StudentInfo *student_info);
        static void ReadIn(vector<StudentInfo*> &students_info, const char *filepath);
        static StudentInfo *ReadInOne(const char *filepath);
        static void GetProperties(StudentInfo *a,
                                  StudentInfo *b,
                                  vector<double> &properties);
        static void GetSimilarity(StudentInfo *a,
                                  StudentInfo *b,
                                  double &similarity,
                                  bool &valuable);
};

#endif /* STUDENT_INFO_H */
