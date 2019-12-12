#include <iostream>
#include <cstring>

struct Solution {
    /* penalty of the solution */
    double penalty;

    /* [i]: timeslot assigned to exam i */
    int* exams_timeslot;

    /* [i, j]: 1 if timeslot j is assigned to exam i, 0 otherwise */
    int** exams_timeslot_matrix;
};

class Problem {
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

int main() {

    std::cout << "Hello, World!" << std::endl;
    return 0;
}