#ifndef K_TO_SCHOOLS_HPP
#define K_TO_SCHOOLS_HPP

#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

struct SchoolScore {
	double _score;
	string _name;
	int _id;
	SchoolScore(const double& score, const char* name, const int& id) {
		_score = score;
		_name = name;
		_id = id;
	}
	~SchoolScore() {}
	bool operator < (const SchoolScore& s) const{
		return _score < s._score;
	}
};

class KStudentsToSchools {
public:
	KStudentsToSchools(const vector<unordered_set<int> >& school_ids_for_student,
			const vector<SchoolScore>& school_score);
	~KStudentsToSchools();

	void SchoolRecommendation(const vector<int>& k_students, 
			vector<string>* school_recommendation);	

private:
	// For each student, choice stores all schools that might accept him.
	vector<unordered_set<int> > _school_ids_for_student;
	// School ranking. Better school has smaller score.
	vector<SchoolScore> _school_score;
};

#endif // K_TO_SCHOOLS_HPP
