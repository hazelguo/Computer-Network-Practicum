#include "StudentCluster.h"

#include <cstdlib>
#include <iostream>
//#include <unordered_map>
#include <map>
#include <cmath>

#include "cluster.h"

const int knpass = 30;

StudentCluster::StudentCluster(int num_students,
                 int num_schools,
                 double **distance,
                 const vector<vector<int> >& student_ids_accepted_by_school) {
    _num_students = num_students;
    _num_schools = num_schools;
    _distance = distance;
    _student_ids_accepted_by_school = student_ids_accepted_by_school;

    // Initialize other member variables.
    _cluster_ids_for_k_value.clear();

    //_k_value_for_school.clear();
    //for (int i = 0; i < _num_schools; ++i) {
    //    _k_value_for_school.push_back(0);
    //}

    _school_ids_for_student.clear();
}

StudentCluster::~StudentCluster() {
//    free(_distance);
    while (!_cluster_ids_for_k_value.empty()) {
        free(_cluster_ids_for_k_value.back());
        _cluster_ids_for_k_value.pop_back();
    }
}

// Public methods for class StudentCluster.

int StudentCluster::GetNumStudents() {
    return _num_students;
}

//unordered_set<int> StudentCluster::GetSchoolIdsForStudent(int student_id) {
set<int> StudentCluster::GetSchoolIdsForStudent(int student_id) {
    if (_school_ids_for_student.empty()) {
        CalcClusterResult();
        for (int school_id = 0; school_id < _num_schools; ++school_id) {
            CalcSelectedStudentsForSchool(school_id);
        }
    }
    return _school_ids_for_student[student_id];
}

// Private methods for class StudentCluster.

int StudentCluster::GetCostOfSchoolWithinClusterIds(int school_id, int *cluster_ids) {
		int cost = 0;
    for (int i = 0; i < _student_ids_accepted_by_school[school_id].size(); ++i) {
        for (int j = i+1; j < _student_ids_accepted_by_school[school_id].size(); ++j) {
					int u = _student_ids_accepted_by_school[school_id][i];
					int v = _student_ids_accepted_by_school[school_id][j];
            cost += (cluster_ids[u] != cluster_ids[v]);
        }
    }
    return max(cost, 1);
}

#define sqr(p) ((p)*(p))
#define cube(p) ((p) * (p) * (p))
double StudentCluster::GetMaxCluster(int *cluster_ids) {
/*	double re = 0;
	int sum = 0;
	int num = 0;
	unordered_map<int, int> count;
	count.clear();
	for (int i = 0; i < _num_students; ++i) {
		count[cluster_ids[i]]++;
	}
	for (unordered_map<int, int>::iterator iter = count.begin(); iter != count.end(); ++iter) {
		sum += iter->second;
		++ num;
	}
	double ave = sum / num;
	for (unordered_map<int, int>::iterator iter = count.begin(); iter != count.end(); ++iter) {
		re += sqr(iter->second-ave);
	}
	re /= num;
	cerr << re << endl;
	if (re < 1) {
		re = 1;
	}
	return re;*/

	double re = 0;
	//unordered_map<int, int> count;
	map<int, int> count;
	count.clear();
	for (int i = 0; i < _num_students; ++i) {
		count[cluster_ids[i]]++;
	}
	for (map<int, int>::iterator iter = count.begin(); iter != count.end(); ++iter) {
		re = max(re, iter->second);
	}
	if (re < 1) {
		re = 1;
	}
	return re;
// 返回一个类中是相应school_id的数量的最大值*同一个类中不属于school_id的数量
/*	double re = 1.0;
	map<int, pair<int, int> > count;
	count.clear();
	for (int i = 0; i < _student_ids_accepted_by_school[school_id].size(); ++i) {
		count[cluster_ids[_student_ids_accepted_by_school[school_id][i]]].first++;
	}
	for (int i = 0; i < _num_students; ++i) {
		count[cluster_ids[i]].second++;
	}
	int max_threshold = 0;
	for (map<int, pair<int, int> >::iterator iter = count.begin(); iter != count.end(); ++iter) {
		max_threshold = max(max_threshold, iter->second.first);
	}
	max_threshold = max(1, (int) (max_threshold * 0.5 + 0.5));
	for (map<int, pair<int, int> >::iterator iter = count.begin(); iter != count.end(); ++iter) {
		if (iter->second.first >= max_threshold) {
			re += max(1, _student_ids_accepted_by_school[school_id].size() - iter->second.first) * (1 + iter->second.second - iter->second.first);
		}
	}
	return re;*/
}

void StudentCluster::CalcClusterResult() {
    //unordered_set<int> empty_set;
    set<int> empty_set;
    empty_set.clear();
    for (int i = 0; i < _num_students; ++i) {
        _school_ids_for_student.push_back(empty_set);
    }
    
		double error;
    int ifound;
    int *cluster_ids;
    vector<double> min_cost_for_school(_num_schools, 1.7976931348623158e+308);

	for (int i = 1; i <= _num_students; ++i) {
		cluster_ids = (int*)calloc(_num_students, sizeof(int));
    _cluster_ids_for_k_value.push_back(cluster_ids);
	}	
	cluster_ids = (int*)calloc(_num_students, sizeof(int));
	double max_error = -10;
	for (int pass = 0; pass < knpass; ++pass) {
		for (int i = 1; i <= _num_students; ++i) {
        kmedoids(i, _num_students, _distance, 2, cluster_ids, &error, &ifound);
				if (max_error < -1) {
					max_error = error;
				}
				//cerr << cluster_ids[0] << " " << cluster_ids[1] << endl;	
				//error = max(1, sqrt(error));
        //error = max(1, log(error));
				double M = GetMaxCluster(cluster_ids);
        for (int school_id = 0; school_id < _num_schools; ++school_id) {
						int student_size = _student_ids_accepted_by_school[school_id].size();
						double t = log(max_error) / log(0.5*student_size*(student_size-1));
						double costfunc = GetCostOfSchoolWithinClusterIds(school_id, cluster_ids);
            //double cost = costfunc * sqrt(sqrt(max(error, 1)) * sqrt(M) * sqrt(i));
            //double cost = cube(costfunc) * (1+abs(_student_ids_accepted_by_school[school_id].size()-M))*(1+abs(i-(_num_students>>1)));
						double cost = pow(costfunc, t) +error;
						//if (school_id == 31) {
						//	cerr << school_id << ": " << cost << " " << costfunc << " " << pow(costfunc, t) << " " << sqrt(error) << " " << pow(i, 1.3333333) << endl;
						//}
						if (cost < min_cost_for_school[school_id]) {
              min_cost_for_school[school_id] = cost;
							for (int j = 0; j < _num_students; ++j) {
								_cluster_ids_for_k_value[school_id][j] = cluster_ids[j];
							}
						}
				}
    }
	}

	/*	for (int i = 0; i < _num_schools; ++i) {
			//cerr << _k_value_for_school[i] << ": ";
			//int k = _k_value_for_school[i];
			cerr << i << ": ";
			for (int j = 0; j < _num_students; ++j) {
				if(_cluster_ids_for_k_value[i][j] == _cluster_ids_for_k_value[i][_num_students-1]) {
					cerr << j << " ";
				}
			}
			cerr << endl;
		}*/
}

map<int, int> StudentCluster::GetClusterCountForSchool(int school_id) {
    map<int, int> count_cluster;
    vector<int> student_ids = _student_ids_accepted_by_school[school_id];
    for (int i = 0; i < student_ids.size(); ++i) {
        int student_id = student_ids[i];
        int k_value = school_id;//_k_value_for_school[school_id];
        int *cluster_id_for_student = _cluster_ids_for_k_value[k_value];
        int cluster_id = cluster_id_for_student[student_id];
        count_cluster[cluster_id] ++;
    }
    return count_cluster;
}

int StudentCluster::GetMaxClusterCount(const map<int, int>& count_cluster) {
    int max_cluster_count = 0;
    for (map<int, int>::const_iterator iter = count_cluster.begin();
         iter != count_cluster.end(); ++iter) {
        max_cluster_count = max(max_cluster_count, iter->second);
    }
    return max_cluster_count;
}

set<int> StudentCluster::GetAcceptedClusterIdsForSchool(int school_id,
                                                           int max_cluster_count,
                                                           const map<int, int>& count_cluster) {
   int cluster_count_threshold = max(1, (int) (max_cluster_count * 0.3 + 0.5));
		//int cluster_count_threshold = 1;
    //cerr << school_id << ": " << cluster_count_threshold << endl;
		set<int> accepted_cluster_ids;
    for (map<int, int>::const_iterator iter = count_cluster.begin();
         iter != count_cluster.end(); ++iter) {
        if (iter->second >= cluster_count_threshold) {
            accepted_cluster_ids.insert(iter->first);
        }
    }
    return accepted_cluster_ids;
}

void StudentCluster::UpdateSchoolForStudents(int school_id, const set<int>& accepted_cluster_ids) {
    for (int student_id = 0; student_id < _num_students; ++student_id) {
        int k_value = school_id;//_k_value_for_school[school_id];
        int *cluster_id_for_student = _cluster_ids_for_k_value[k_value];
        int cluster_id = cluster_id_for_student[student_id];
        if (accepted_cluster_ids.find(cluster_id) != accepted_cluster_ids.end()) {
            _school_ids_for_student[student_id].insert(school_id);
        }
    }
}

void StudentCluster::CalcSelectedStudentsForSchool(int school_id) {
    map<int, int> count_cluster = GetClusterCountForSchool(school_id);
    int max_cluster_count = GetMaxClusterCount(count_cluster);
    set<int> accepted_cluster_ids = GetAcceptedClusterIdsForSchool(school_id, max_cluster_count, count_cluster);
    UpdateSchoolForStudents(school_id, accepted_cluster_ids);
}
