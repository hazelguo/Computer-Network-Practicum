#include "k_to_schools.h"

#include <vector>
#include <unordered_set>
#include <string>

using namespace std;

KStudentsToSchools::KStudentsToSchools(
		const vector<unordered_set<int> >& school_ids_for_student,
		const vector<SchoolScore>& school_score) {
	_school_ids_for_student.clear();
	_school_score.clear();
	for (vector<unordered_set<int> >::const_iterator iter = 
			school_ids_for_student.begin(); iter != school_ids_for_student.end(); 
			++iter) {
		_school_ids_for_student.push_back(*iter);
	}
	for (vector<SchoolScore>::const_iterator iter = 
			school_score.begin(); iter != school_score.end(); ++iter) {
		_school_score.push_back(*iter);
	}
}

KStudentsToSchools::~KStudentsToSchools() {}

void KStudentsToSchools::SchoolRecommendation(const vector<int>& k_students,
		vector<string>* school_recommendation) {
	unordered_set<int> potential_schools;
	
	potential_schools.clear();
	for (vector<int>::const_iterator student = k_students.begin(); 
			student != k_students.end(); ++student) {
		for (unordered_set<int>::const_iterator school = 
				_school_ids_for_student[*student].begin(); 
				school != _school_ids_for_student[*student].end();
				school ++) {
			potential_schools.insert(*school);
		}
	}
	
	for (vector<SchoolScore>::iterator school = _school_score.begin();
			school != _school_score.end(); ++school) {
		if (potential_schools.find(school->_id) != potential_schools.end()) {
			school_recommendation->push_back(school->_name);
		}
	}	
}
