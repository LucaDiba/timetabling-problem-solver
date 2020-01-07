#ifndef DATA_STRUCTURES_CPP
#define DATA_STRUCTURES_CPP

#include <string>
#include <vector>
#include <array>
#include <list>

#include "Exam.h"

class Solution {

public:

    /* [i]: timeslot assigned to exam i */
    std::vector<Exam*> *exams;
    int timeslots, students;
    int *examsTimeslots;
    std::vector<std::vector<int>> timeslotsExams;

    /* Penalty of the solution */
    double penalty = 0.0;
    double gain = 0.0;

    /* Solution feasibility */
    bool isFeasible = true;

    /* Constructor */
    Solution(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents);

    /* Feasibility checker */
    bool getFeasibility();

private:

    void computePenalty();

};

#endif //DATA_STRUCTURES_CPP
