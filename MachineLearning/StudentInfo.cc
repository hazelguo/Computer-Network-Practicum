#include "StudentInfo.h"

#include <iostream>
#include <cmath>

#define cube(p) ((p)*(p)*(p))
double sqr(double a) {
    return a * a;
}

vector<School*> School::C_school;
vector<School*> School::A_school;

/*
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
    int id;
    char s[100];
    fp_china = fopen(filepath_china, "r");
    fp_USA = fopen(filepath_USA, "r");
    while(fscanf(fp_china, "%d%lf", &id, &rank) > 0) {
        china_school.push_back(rank);
    }
    while(fscanf(fp_USA, "%d%lf", &id, &rank) > 0) {
        USA_school.push_back(rank);
    }
    fclose(fp_china);
    fclose(fp_USA);
}
*/

School::School(double _rank, string _name) {
    rank = _rank;
    name = _name;
}

void School::ReadIn(vector<School*> &schools,
                    const char *filepath) {
    FILE *fp;
    int id;
    double rank;
    char name[80];
    School* school = NULL;
    fp = fopen(filepath, "r");
    while (fscanf(fp, "%d%lf", &id, &rank) > 0) {
        fgets(name, 100, fp);
        school = new School(rank, name);
        schools.push_back(school);
    }
}

void School::ReadIn(const char *filepath_A,
                    const char *filepath_C) {
    ReadIn(A_school, filepath_A);
    ReadIn(C_school, filepath_C);
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

int StudentInfo::attributes_num = 10;

vector<double> StudentInfo::total(StudentInfo::attributes_num, 0);
vector<int> StudentInfo::count(StudentInfo::attributes_num, 0);
vector<double> StudentInfo::average(StudentInfo::attributes_num, 0);
vector<double> StudentInfo::std(StudentInfo::attributes_num, 0);

StudentInfo::StudentInfo(int _school, double _GPA, double _IELTS, int _TOEFL,
                         double _GRE_overall, double _GRE_verbal, double _GRE_writing,
                         int _research_intern, int _company_intern,
                         int _paper, int _final_decision) {
    srand(time(0));

		school = _school;
    GPA = _GPA;
    IELTS = _IELTS;
    TOEFL = _TOEFL;
    GRE_overall = _GRE_overall;
    GRE_verbal = _GRE_verbal;
    GRE_writing = _GRE_writing;
    research_intern = _research_intern;
    company_intern = _company_intern;
    //paper = _paper;
		paper = 0;
		int paper_num = (int)((double)_paper*ceil(sqrt((double)(rand()%100+1)))*0.1); // 1-1, 234-2, 56789-3		
		for (int i = 0; i < paper_num; ++i) {
			int paper_level = (int)ceil(sqrt((double)(rand()%196+1)));
			if (paper_level == 1) {
				paper += 10;
			} else if (paper_level == 2) {
				paper += 8;
			} else if (paper_level == 3) {
				paper += 6;
			} else if (paper_level == 4) {
				paper += 6;
			} else if (paper_level == 5) {
				paper += 4;
			} else if (paper_level == 6) {
				paper += 4;
			} else if (paper_level == 7) {
				paper += 2;
			} else if (paper_level == 8) {
				paper += 2;
			} else if (paper_level == 9) {
				paper += 1;
			} else if (paper_level == 10) {
				paper += 1;
			}	
		}	
		cerr << paper << endl;
 
		final_decision = _final_decision;
    offers.clear();
    attributes.clear();
    attributes.push_back(School::C_school[school]->rank);
    attributes.push_back(GPA);
    attributes.push_back(IELTS);
    attributes.push_back(TOEFL);
    attributes.push_back(GRE_overall);
    attributes.push_back(GRE_verbal);
    attributes.push_back(GRE_writing);
    attributes.push_back(research_intern);
    attributes.push_back(company_intern);
    attributes.push_back(paper);
}
/*
void StudentInfo::CalculateAverage() {
    for (int i = 0; i < attributes_num; ++i) {
        if (count[i] == 0) average[i] = 0;
        else average[i] = total[i] / count[i];
    }
}

void StudentInfo::Normalize() {
    for (i = 0; i < attributes.size(); i++) {
        if (attributes[i] < 0){
            attributes[i] = average[i];
        }
    }
}
*/

void StudentInfo::Standardize(vector<StudentInfo*> &students_info) {
    //vector<double> total(attributes_num, 0);
    //vector<double> count(attributes_num, 0);
    //vector<double> average(attributes_num, 0);
    //vector<double> std(attributes_num, 0);
    for (int i = 0; i < students_info.size(); ++i) {
        for (int j = 0; j < attributes_num; ++j) {
            if (students_info[i]->attributes[j] >= -0.0001) {
                total[j] += students_info[i]->attributes[j];
                count[j]++;
            }
        }
    }
    for (int j = 0; j < attributes_num; ++j) {
        average[j] = total[j] / count[j];
    }
    for (int i = 0; i < students_info.size(); ++i) {
        for (int j = 0; j < attributes_num; ++j) {
            if (students_info[i]->attributes[j] >= -0.0001) {
                std[j] += sqr(students_info[i]->attributes[j] - average[j]);
            }
        }
    }
    for (int j = 0; j < attributes_num; ++j) {
        std[j] /= (count[j] - 1);
        std[j] = sqrt(std[j]);
    }
    for (int i = 0; i < students_info.size(); ++i) {
        for (int j = 0; j < attributes_num; ++j) {
            if (students_info[i]->attributes[j] >= -0.0001) {
                students_info[i]->attributes[j] =
                    (students_info[i]->attributes[j] - average[j]) / std[j];
            }
            else {
                students_info[i]->attributes[j] = 0;
            }
					//	printf("%lf ", students_info[i]->attributes[j]);
        }
			//printf("\n");
    }
}

void StudentInfo::Standardize(StudentInfo *student_info) {
    for (int j = 0; j < attributes_num; ++j) {
        if (student_info->attributes[j] >= -0.0001) {
            student_info->attributes[j] =
                (student_info->attributes[j] - average[j]) / std[j];
        }
        else {
            student_info->attributes[j] = 0;
        }
    }
}

void StudentInfo::ReadIn(vector<StudentInfo*> &students_info,
                         const char *filepath) {
    FILE *fp;
    StudentInfo* student_info = NULL;
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
    int _final_decision;
    fp = fopen(filepath, "r");
    while (fscanf(fp, "%d%lf%lf%d%lf%lf%lf%d%d%d%d", &_school,
                  &_GPA, &_IELTS, &_TOEFL, &_GRE_overall, &_GRE_verbal,
                  &_GRE_writing, &_research_intern, &_company_intern, 
                  &_paper, &_final_decision) > 0) {
        student_info = new StudentInfo(_school, _GPA, _IELTS, _TOEFL,
                                       _GRE_overall, _GRE_verbal, _GRE_writing, 
                                       _research_intern, _company_intern,
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
    double _GRE_overall;
    double _GRE_verbal;
    double _GRE_writing;
    int _research_intern;
    int _company_intern;
    int _paper;
    fp = fopen(filepath, "r");
    fscanf(fp, "%d%lf%lf%d%lf%lf%lf%d%d%d", &_school,
                  &_GPA, &_IELTS, &_TOEFL, &_GRE_overall,
                  &_GRE_verbal, &_GRE_writing, &_research_intern, 
                  &_company_intern, &_paper);
    student_info = new StudentInfo(_school, _GPA, _IELTS, _TOEFL,
                                   _GRE_overall, _GRE_verbal, _GRE_writing, 
                                   _research_intern, _company_intern,
                                   _paper, 0);
    fclose(fp);
    return student_info;
}

void StudentInfo::GetProperties(StudentInfo *a,
                                StudentInfo *b,
                                vector<double> &properties) {
    properties.clear();
    for (int i = 0; i < StudentInfo::attributes_num; ++i){
        properties.push_back(a->attributes[i] - b->attributes[i]);
    }
}

void StudentInfo::GetSimilarity(StudentInfo *a, 
                                StudentInfo *b,
                                double &similarity,
                                bool &valuable) {
		similarity = 700*cube(b->attributes[0]-a->attributes[0]) 
							+ 1000.0*cube(a->attributes[1]-b->attributes[1]) 
							+ 100.0*(a->attributes[3]-b->attributes[3])
							+ 50*(a->attributes[4]-b->attributes[4]) 
							+ 50*(a->attributes[5]-b->attributes[5])
							+ 10*(a->attributes[6]-b->attributes[6]) 
							+ 300*cube(a->attributes[7]-b->attributes[7])
							+ 300*cube(a->attributes[8]-b->attributes[8])
							+ 500*cube(a->attributes[9]-b->attributes[9]);

		valuable = 1;
		return;
			

    int same_offer_num = 0;
    int size_a = a->offers.size();
    int size_b = b->offers.size();
    double min_a = 50;
    double min_b = 50;
    for (int i = 0; i < size_a; ++i){
        if (School::A_school[a->offers[i].school]->rank >= -0.0001) {
            min_a = min(min_a, School::A_school[a->offers[i].school]->rank 
                * (1 + (-0.2 * a->offers[i].degree)));
        }
    }
    for (int i = 0; i < size_b; ++i){
        if (School::A_school[b->offers[i].school]->rank >= -0.0001) {
            min_b = min(min_b, School::A_school[b->offers[i].school]->rank 
                * (1 + (-0.2 * b->offers[i].degree)));
        }
    }
    /*
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
    */
    valuable = true;
    similarity = (min_a - min_b) / 50.0;
}
