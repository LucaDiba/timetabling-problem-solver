#ifndef DATA_STRUCTURES_CPP
#define DATA_STRUCTURES_CPP

#include <string>

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

    Solution* best_solution;
    Solution* current_solution;
};

/**
 * Check the feasibility of the given solution on the given problem
 * @param problem
 * @param solution
 * @return feasibility (true: feasible, false: unfeasible)
 */
bool checkFeasibility(Problem* problem, Solution solution) {
    // TO-DO:
    return false;
}

/**
 * Compute the penalty of the given solution on the given problem
 * @param problem
 * @param solution
 * @return penalty
 */
double computePenalty(Problem* problem, Solution solution) {
    return NAN;
}

#endif //DATA_STRUCTURES_CPP