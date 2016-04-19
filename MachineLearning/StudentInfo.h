#ifndef STUDENT_INFO_H
#define STUDENT_INFO_H

#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

class School {
public:
    static vector<double> china_school;
    static vector<double> USA_school;
    static void ReadIn(const char *filepath_china, const char *filepath_USA);
    static double GetChinaSchoolRank(int id);
    static double GetUSASchoolRank(int id);

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
        int GRE;
        double GRE_writing;
        int summer_intern;
        int paper;
        int final_decision;
        vector<Offer> offers;
        StudentInfo(int _school, double _GPA, double _IELTS, int _TOEFL,
                    int _GRE, double _GRE_writing, int _summer_intern,
                    int _paper, int _final_decision);
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
