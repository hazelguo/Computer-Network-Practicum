#include "cluster.h"
#include <vector>
#include <limits.h>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

const int knpass = 10;

class Cluster {
public:
    Cluster(int num_students,
            int num_schools,
            double **distance,
            const vector<vector<int> >& student_ids_accepted_by_school);
    ~Cluster();

    /**
     * Returns number of students.
     */
    int GetNumStudents();

    /**
     * Gets potential schools for each student.
     *
     * @param student_id student id of the given student.
     * @return a set of school ids for the given student.
     */
    unordered_set<int> GetSchoolIdsForStudent(int student_id);

private:
    /**
     * Calculates cost function for school 'school_id' within cluster result 'cluster_ids'.
     *
     * @param school_id school id of the given school.
     * @param cluster_ids a list of cluster ids for the given cluster result.
     * @return the cost function result.
     */
    int GetCostOfSchoolWithinClusterIds(int school_id, int *cluster_ids);

    /**
     * Runs kmedois n times and calculates optimal K for each school.
     */
    void CalcClusterResult();

    /**
     * Summarizes the amount of accepted students in each cluster.
     *
     * @param school_id school id of the given school.
     * @return a map of count for each cluster id.
     */
    unordered_map<int, int> GetClusterCountForSchool(int school_id);

    /**
     * Finds the maximum of accepted students in one cluster.
     * 
     * @param count_cluster the map of count for each cluster id.
     * @return maximum of accepted students in one cluster.
     */
    int GetMaxClusterCount(const unordered_map<int, int>& count_cluster);

    /**
     * Calculates the cluster ids which are acceptible for school 'school_id'.
     *
     * @param school_id school id of the given school.
     * @param max_cluster_count maximum of accepted students in one cluster.
     * @param count_cluster the map of count for each cluster id.
     * @return a set of cluster ids which are acceptible for the given school.
     */
    unordered_set<int> GetAcceptedClusterIdsForSchool(int school_id,
                                                      int max_cluster_count,
                                                      const unordered_map<int, int>& count_cluster);

    /**
     * Updates choice of the given school for students based on 'accepted_cluster_ids'.
     *
     * @param school_id school id of the given school.
     * @param accepted_cluster_ids a set of cluster ids which are acceptible for the given school.
     */
    void UpdateSchoolForStudents(int school_id, const unordered_set<int>& accepted_cluster_ids);

    /**
     * Calculates all students that school 'school_id' might accept, store the result in _school_ids_for_student.
     *
     * @param school_id school id of the given school.
     */
    void CalcSelectedStudentsForSchool(int school_id);

    // Students number.
    int _num_students;
    // Schools number.
    int _num_schools;
    // Distance matrix between all students.
    double **_distance;
    // For each school, the id of students that they accepted.
    vector<vector<int> > _student_ids_accepted_by_school;

    // Cluster result (id array) for each K value.
    vector<int *> _cluster_ids_for_k_value;
    // Optimal K value in K-medoids algorithm for each school.
    vector<int> _k_value_for_school;
    // For each student, Choice stores all schools that might accpet him.
    vector<unordered_set<int> > _school_ids_for_student;
};

Cluster::Cluster(int num_students,
                 int num_schools,
                 double **distance,
                 const vector<vector<int> >& student_ids_accepted_by_school) {
    _num_students = num_students;
    _num_schools = num_schools;
    _distance = distance;
    _student_ids_accepted_by_school = student_ids_accepted_by_school;

    // Initialize other member variables.
    _cluster_ids_for_k_value.clear();

    _k_value_for_school.clear();
    for (int i = 0; i < _num_schools; ++i) {
        _k_value_for_school.push_back(0);
    }

    _school_ids_for_student.clear();
    unordered_set<int> empty_set;
    empty_set.clear();
    for (int i = 0; i < _num_students; ++i) {
        _school_ids_for_student.push_back(empty_set);
    }
}

Cluster::~Cluster() {
    free(_distance);
    while (!_cluster_ids_for_k_value.empty()) {
        free(_cluster_ids_for_k_value.back());
        _cluster_ids_for_k_value.pop_back();
    }
}

// Public methods for class Cluster.

int Cluster::GetNumStudents() {
    return _num_students;
}

unordered_set<int> Cluster::GetSchoolIdsForStudent(int student_id) {
    if (_school_ids_for_student.empty()) {
        CalcClusterResult();
        for (int school_id = 0; school_id < _num_schools; ++school_id) {
            CalcSelectedStudentsForSchool(school_id);
        }
    }
    return _school_ids_for_student[student_id];
}

// Private methods for class Cluster.

int Cluster::GetCostOfSchoolWithinClusterIds(int school_id, int *cluster_ids) {
    int cost = 1;
    for (int i = 0; i < _student_ids_accepted_by_school[school_id].size(); ++i) {
        for (int j = i+1; j < _student_ids_accepted_by_school[school_id].size(); ++j) {
            cost += (cluster_ids[i] != cluster_ids[j]);
        }
    }
    return cost;
}

void Cluster::CalcClusterResult() {
    double error;
    int ifound;
    int *cluster_ids;
    vector<double> min_cost_for_school(_num_schools, (double)INT_MAX);

    for (int i = 1; i <= _num_students; ++i) {
        cluster_ids = (int*)calloc(_num_students, sizeof(int));
        kmedoids(i, _num_students, _distance, knpass, cluster_ids, &error, &ifound);
        _cluster_ids_for_k_value.push_back(cluster_ids);

        error = max(1, log(max(error, 1)));
        for (int school_id = 0; school_id < _num_schools; ++school_id) {
            double cost = GetCostOfSchoolWithinClusterIds(school_id, cluster_ids) * error;
            if (cost < min_cost_for_school[school_id]) {
                min_cost_for_school[school_id] = cost;
                _k_value_for_school[school_id] = i-1;
            }
        }
    }
}

unordered_map<int, int> Cluster::GetClusterCountForSchool(int school_id) {
    unordered_map<int, int> count_cluster;
    vector<int> student_ids = _student_ids_accepted_by_school[school_id];
    for (int i = 0; i < student_ids.size(); ++i) {
        int student_id = student_ids[i];
        int k_value = _k_value_for_school[school_id];
        int *cluster_id_for_student = _cluster_ids_for_k_value[k_value];
        int cluster_id = cluster_id_for_student[student_id];
        count_cluster[cluster_id] ++;
    }
    return count_cluster;
}

int Cluster::GetMaxClusterCount(const unordered_map<int, int>& count_cluster) {
    int max_cluster_count = 0;
    for (unordered_map<int, int>::const_iterator iter = count_cluster.begin();
         iter != count_cluster.end(); ++iter) {
        max_cluster_count = max(max_cluster_count, iter->second);
    }
    return max_cluster_count;
}

unordered_set<int> Cluster::GetAcceptedClusterIdsForSchool(int school_id,
                                                           int max_cluster_count,
                                                           const unordered_map<int, int>& count_cluster) {
    int cluster_count_threshold = (int) (max_cluster_count * 0.5 + 0.5);
    unordered_set<int> accepted_cluster_ids;
    for (unordered_map<int, int>::const_iterator iter = count_cluster.begin();
         iter != count_cluster.end(); ++iter) {
        if (iter->second >= cluster_count_threshold) {
            accepted_cluster_ids.insert(iter->first);
        }
    }
    return accepted_cluster_ids;
}

void Cluster::UpdateSchoolForStudents(int school_id, const unordered_set<int>& accepted_cluster_ids) {
    for (int student_id = 0; student_id < _num_students; ++student_id) {
        int k_value = _k_value_for_school[school_id];
        int *cluster_id_for_student = _cluster_ids_for_k_value[k_value];
        int cluster_id = cluster_id_for_student[student_id];
        if (accepted_cluster_ids.find(cluster_id) != accepted_cluster_ids.end()) {
            _school_ids_for_student[student_id].insert(school_id);
        }
    }
}

void Cluster::CalcSelectedStudentsForSchool(int school_id) {
    unordered_map<int, int> count_cluster = GetClusterCountForSchool(school_id);
    int max_cluster_count = GetMaxClusterCount(count_cluster);
    unordered_set<int> accepted_cluster_ids = GetAcceptedClusterIdsForSchool(school_id, max_cluster_count, count_cluster);
    UpdateSchoolForStudents(school_id, accepted_cluster_ids);
}

// Local methods for main.

Cluster ConstructClusterFromInput() {
    int num_students, num_schools;
    scanf("%d%d", &num_students, &num_schools);

    double **distance = (double**)calloc(num_students, sizeof(double*));
    for (int i = 0; i < num_students; ++i) {
        distance[i] = (double*)calloc(i, sizeof(double));
        for (int j = 0; j < i; ++j) {
            scanf("%lf", &distance[i][j]);
        }
    }

    vector<vector<int> > student_ids_accepted_by_school;
    for (int i = 0; i < num_schools; ++i) {
        vector<int> students;
        students.clear();
        int j, k;
        for (scanf("%d", &j); j ; --j) {
            scanf("%d", &k);
            students.push_back(k);
        }
        student_ids_accepted_by_school.push_back(students);
    }

    Cluster cluster(num_students, num_schools, distance, student_ids_accepted_by_school);
    return cluster;
}

void OutputSchoolsForStudents(Cluster cluster) {
    int num_students = cluster.GetNumStudents();
    for (int student_id = 0; student_id < num_students; ++student_id) {
        unordered_set<int> school_ids = cluster.GetSchoolIdsForStudent(student_id);
        printf("%lu ", school_ids.size());
        for (unordered_set<int>::iterator iter = school_ids.begin(); iter != school_ids.end(); ++iter) {
            printf("%d ", *iter);
        }
        printf("\n");
    }
}

int main() {
    Cluster cluster = ConstructClusterFromInput();
    OutputSchoolsForStudents(cluster);
    return 0;
}
