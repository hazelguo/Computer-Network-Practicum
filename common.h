#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>

using namespace std;

struct Offer {
	int uni;
	int degree;
};

class Info {
	public:
		int city;
		int uni;
		double GPA;
		double IELTS;
		int TOEFL;
		int GRE;
		int GRE_Writing;
		int summer_intern;
		int paper;
		string final_decision;
		vector<Offer> offers;
};

#endif // COMMON_H
