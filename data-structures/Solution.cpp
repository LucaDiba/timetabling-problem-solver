#include <algorithm>
#include <iterator>
#include <random>
#include <vector>
#include <limits>

#include "Exam.h"
#include "Solution.h"


/* Solution class */

Solution::Solution(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents) {

    /* Store collection length */
    exams = examsVector;
    timeslots = numberOfTimeslots;
    students = numberOfStudents;

    /* Generate collections for exams/timeslots mapping and vice-versa */
    examsTimeslots = new int[exams->size()];

    /* Initialize timeslots/exams vector */
    for(int i = 0; i < timeslots; i++)
        timeslotsExams.emplace_back(std::vector<int>());

}

Solution::Solution(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents, int *initializingSolution){

    /* Store collection length */
    exams = examsVector;
    timeslots = numberOfTimeslots;
    students = numberOfStudents;

    /* Generate collections for exams/timeslots mapping and vice-versa */
    examsTimeslots = initializingSolution;

    /* Initialize timeslots/exams vector */
    for(int i = 0; i < timeslots; i++)
        timeslotsExams.emplace_back(std::vector<int>());

};

bool Solution::getFeasibility(bool evaluatePenalty, int start, int end) {

    /* Populate timeslots/exam vector of lists */
    for(int i = start; i < (end > 0 ? end : exams->size()); i++)
        timeslotsExams[examsTimeslots[i]].push_back(i);

    /* Check feasibility */
    for(auto& timeslot: timeslotsExams){

        for(int i = 0; i < timeslot.size() && isFeasible; i++){
            for(int j = i; j < timeslot.size() && isFeasible; j++){
                if((*exams)[timeslot[i]]->hasConflict(timeslot[j])) {
                    penalty = std::numeric_limits<double>::max();
                    isFeasible = false;
                }
            }
        }

        if(!isFeasible)
            break;

    }

    if(isFeasible && evaluatePenalty)
        computePenalty();

    return isFeasible;

}

bool Solution::getCutFeasibility(int minCut, int maxCut) {

    // Evaluate only cutting section feasibility
    return getFeasibility(false, minCut, maxCut);

}

void Solution::initializeRandomSolution() {

    // Initialize exams population
    int timeslotSample[exams->size()];

    // Initialize randomizer
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(0, timeslots - 1);

    for(int i = 0; i < exams->size(); i++)
        // Extract random integer and fill up timeslot sample
        examsTimeslots[i] = distribution(generator);

}

void Solution::computePenalty() {

    // Compute penalty
    for(int i = 0; i < exams->size(); i++){
        for(int j = i + 1; (j < i + 5) && (j < exams->size()) ; j++){
            if((*exams)[i]->hasConflict(j))
                penalty += pow(2, (5 - abs(examsTimeslots[j] - examsTimeslots[i]))) * (*exams)[i]->getConflict(j) / students;
        }
    }

    // Store inverse penalty due to maximization problem
    if (penalty == 0) { // avoid dividing by zero
        gain = std::numeric_limits<double>::max();
    } else {
        gain = 1.0 / penalty;
    }
}
