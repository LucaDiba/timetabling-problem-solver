#include <iostream>
#include <string>
#include <utility>
#include <ctime>

struct Solution {
    /* penalty of the solution */
    double penalty;

    /* [i]: timeslot assigned to exam i */
    int* exams_timeslot;

    /* [i, j]: 1 if timeslot j is assigned to exam i, 0 otherwise */
    int** exams_timeslot_matrix;
};

struct Problem {
    /* instance name */
    std::string instance_name;

    /* max time in seconds */
    int max_time;

    /* timestamp: when the program has started */
    int start_time;

    /* number of exams */
    int n_exams;

    /* number of students */
    int n_students;

    /* number of timeslots */
    int n_timeslots;

    /* [i, j]: 1 if student i is enrolled in exam j, 0 otherwise */
    int** student_enrolled;

    /* [i, j]: number of students enrolled in both exams i and j */
    int** conflicts;

    Solution best_solution;
    Solution current_solution;
};

double computePenaltyOfCurrentSolution(Problem* p) {
    return NAN;
}

void printSolutionOnFile(Solution sol) {
}

bool checkFeasibility(Solution sol) {
    return false;
}

Problem* getProblemFromFile(std::string instance_name, int max_time) {
    Problem* p = new Problem;

    p->instance_name = std::move(instance_name);
    p->max_time = max_time;
    p->start_time = time(NULL);

    // TO-DO: complete

    return p;
}

int main(int argc, char * argv[]) {
    std::string instance_name = argv[1];
    int max_time = atoi(argv[3]);
    Problem* problem = getProblemFromFile(instance_name, max_time);
    return 0;
}