#include "StudentInfo.h"

#include <iostream>

vector<double> School::china_school;
vector<double> School::USA_school;

double School::GetChinaSchoolRank(int id) {
    return china_school[id];
}

double School::GetUSASchoolRank(int id) {
    return USA_school[id];
}

void School::ReadIn(const char *filepath_china,
                    const char *filepath_USA) {
    FILE *fp_china, *fp_USA;
    double rank;
    fp_china = fopen(filepath_china, "r");
    fp_USA = fopen(filepath_USA, "r");
    while(fscanf(fp_china, "%lf", &rank) > 0) {
        china_school.push_back(rank);
    }
    while(fscanf(fp_USA, "%lf", &rank) > 0) {
        USA_school.push_back(rank);
    }
    fclose(fp_china);
    fclose(fp_USA);
}

Offer::Offer(int _school, int _degree) {
    school = _school;
    degree = _degree;
}

bool Offer::operator<(const Offer &a) const {
    if (school != a.school)
        return school < a.school;
    else return degree < a.degree;
}

bool Offer::operator==(const Offer &a) const {
    if ((school == a.school) && (degree == a.degree))
        return true;
    else return false;
}

StudentInfo::StudentInfo(int _school, double _GPA, double _IELTS, int _TOEFL,
                         int _GRE, double _GRE_writing, int _summer_intern,
                         int _paper, int _final_decision) {
    school = _school;
    GPA = _GPA;
    IELTS = _IELTS;
    TOEFL = _TOEFL;
    GRE = _GRE;
    GRE_writing = _GRE_writing;
    summer_intern = _summer_intern;
    paper = _paper;
    final_decision = _final_decision;
    offers.clear();
}

void StudentInfo::ReadIn(vector<StudentInfo*> &students_info,
                         const char *filepath){
    FILE *fp;
    StudentInfo* student_info = NULL;
    int _school;
    double _GPA;
    double _IELTS;
    int _TOEFL;
    int _GRE;
    double _GRE_writing;
    int _summer_intern;
    int _paper;
    int _final_decision;
    fp = fopen(filepath, "r");
    while (fscanf(fp, "%d%lf%lf%d%d%lf%d%d%d", &_school,
                  &_GPA, &_IELTS, &_TOEFL, &_GRE, &_GRE_writing,
                  &_summer_intern, &_paper, &_final_decision) > 0) {
        student_info = new StudentInfo(_school, _GPA, _IELTS, _TOEFL,
                                       _GRE, _GRE_writing, _summer_intern,
                                       _paper, _final_decision);
        int _offer_num, _school, _degree;
        fscanf(fp, "%d", &_offer_num);
        while (_offer_num --) {
            fscanf(fp, "%d%d", &_school, &_degree);
            student_info->offers.push_back(Offer(_school, _degree));
            /*
            // sort the offers
            sort(student_info->offers.begin(), student_info->offers.end());
            */
        }
        students_info.push_back(student_info);
    }
    fclose(fp);
}

StudentInfo *StudentInfo::ReadInOne(const char *filepath) {
    FILE *fp;
    StudentInfo *student_info = NULL;
    int _school;
    double _GPA;
    double _IELTS;
    int _TOEFL;
    int _GRE;
    double _GRE_writing;
    int _summer_intern;
    int _paper;
    fp = fopen(filepath, "r");
    fscanf(fp, "%d%lf%lf%d%d%lf%d%d", &_school,
           &_GPA, &_IELTS, &_TOEFL, &_GRE, &_GRE_writing,
           &_summer_intern, &_paper);
    student_info = new StudentInfo(_school, _GPA, _IELTS, _TOEFL,
                                   _GRE, _GRE_writing, _summer_intern,
                                   _paper, 0);
    fclose(fp);
    return student_info;
}

void StudentInfo::GetProperties(StudentInfo *a,
                                StudentInfo *b,
                                vector<double> &properties) {
    properties.clear();
    properties.push_back(School::GetChinaSchoolRank(a->school) - 
            School::GetChinaSchoolRank(b->school));
    properties.push_back(a->GPA - b->GPA);
    properties.push_back(a->IELTS - b->IELTS);
    properties.push_back(a->GRE - b->GRE);
    properties.push_back(a->GRE_writing - b->GRE_writing);
    properties.push_back(a->summer_intern - b->summer_intern);
    properties.push_back(a->paper - b->paper);
}

void StudentInfo::GetSimilarity(StudentInfo *a, 
                                StudentInfo *b,
                                double &similarity,
                                bool &valuable) {
    int same_offer_num = 0;
    int size_a = a->offers.size();
    int size_b = b->offers.size();
    int pointer_a = 0, pointer_b = 0;
    while ((pointer_a < size_a) && (pointer_b < size_b)) {
        if (a->offers[pointer_a] == b->offers[pointer_b]) {
            same_offer_num ++;
            pointer_a ++;
            pointer_b ++;
        }
        else if (a->offers[pointer_a] < b->offers[pointer_b])
            pointer_a ++;
        else pointer_b ++;
    }
    if (same_offer_num == 0) valuable = false;
    else valuable = true;
    similarity = 1;
    while (same_offer_num > 0) {
        similarity /= 2;
        same_offer_num --;
    }
}
