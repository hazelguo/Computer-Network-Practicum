#ifndef STUDENT_CLUSTER_H
#define STUDENT_CLUSTER_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class StudentCluster {
public:
    StudentCluster(int num_students,
            int num_schools,
            double **distance,
            const vector<vector<int> >& student_ids_accepted_by_school);
    ~StudentCluster();

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
		double GetMaxCluster(int *cluster_ids);

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
    // For each student, Choice stores all schools that might accept him.
    vector<unordered_set<int> > _school_ids_for_student;
};

#endif /* STUDENT_CLUSTER_H */
