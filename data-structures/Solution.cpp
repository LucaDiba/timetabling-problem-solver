#include <algorithm>
#include <iterator>
#include <random>
#include <vector>
#include <limits>

#include "Exam.h"
#include "Solution.h"
#include "rand.h"

/* Solution class */

Solution::Solution(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents) {

    /* Store collection length */
    exams = examsVector;
    timeslots = numberOfTimeslots;
    students = numberOfStudents;

    /* Initialize exams vector */
    examsTimeslots = new int[exams->size()];

}

Solution::Solution(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents, int *initializingSolution){

    /* Store collection length */
    exams = examsVector;
    timeslots = numberOfTimeslots;
    students = numberOfStudents;

    /* Initialize timeslots/exams vector */
    for(int i = 0; i < timeslots; i++)
        timeslotsExams.emplace_back(std::vector<int>());

    /* Generate collections for exams/timeslots mapping and vice-versa */
    examsTimeslots = new int[exams->size()];
    for(int i = 0; i < examsVector->size(); ++i) {
        /* Copy exam's timeslot */
        examsTimeslots[i] = initializingSolution[i];

        /* Add exam to its timeslot */
        timeslotsExams[examsTimeslots[i]].push_back(i);
    }

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
                    penalty = 1000;//std::numeric_limits<double>::max();
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

void displayy(std::vector<int> a) //TODO: remove after debug
{
    for (int i = 0; i < a.size(); i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void Solution::initializeRandomSolution(bool feasible) {

    if(feasible) {
        bool found_infeasibility = true;
        std::vector<int> tmp_examsTimeslots(exams->size());
        std::vector<int> shuffled_exams(exams->size());
        for (int i = 0; i < exams->size(); ++i) {
            shuffled_exams[i] = i;
        }

        int try_n=0;
        while(found_infeasibility) {
            found_infeasibility = false;
            std::uniform_int_distribution<int> exams_distribution(0, exams->size() - 1);

            std::fill(tmp_examsTimeslots.begin(), tmp_examsTimeslots.end(), -1);
            std::vector<std::vector<int>> tmp_timeslotsExams;
            /* Initialize timeslots/exams vector */
            for(int i = 0; i < timeslots; ++i)
                tmp_timeslotsExams.emplace_back(std::vector<int>());

            std::shuffle(std::begin(shuffled_exams), std::end(shuffled_exams), generator);
//            displayy(shuffled_exams);

            // For each exam
            int i;
            for (i = 0; i < shuffled_exams.size() && !found_infeasibility; ++i){
                int curr_exam = shuffled_exams[i];

                // Search for a timeslot until you find one with no conflicts
                for (int j = 0; j < timeslots && tmp_examsTimeslots[curr_exam] == -1; ++j) {
                    found_infeasibility = false;

                    // Scroll all exams in the current timeslot
                    for (int k = 0; k < tmp_timeslotsExams[j].size() && !found_infeasibility; ++k) {
                        int curr_exam_in_timeslot = tmp_timeslotsExams[j][k];
                        if((*exams)[curr_exam]->hasConflict(curr_exam_in_timeslot)) {
//                            printf("\t\texam %d with exam %d\n", i, shuffled_exams[k]);
                            found_infeasibility = true;
                        }
                    }

                    if (!found_infeasibility) {
                        tmp_examsTimeslots[curr_exam] = j;
                        tmp_timeslotsExams[j].emplace_back(i);
                    }
                }
            }
//            printf("Try %d \t blocked at %d of %d (%d)\n", ++try_n, i, exams->size(), shuffled_exams.size());
        }


        /* Initialize timeslots/exams vector */
        timeslotsExams.clear();
        for(int i = 0; i < timeslots; i++) {
            timeslotsExams.emplace_back(std::vector<int>());
        }

        /* Generate collections for exams/timeslots mapping and vice-versa */
        examsTimeslots = new int[exams->size()];
        for(int i = 0; i < exams->size(); ++i) {
            /* Copy exam's timeslot */
            examsTimeslots[i] = tmp_examsTimeslots[i];

            /* Add exam to its timeslot */
            timeslotsExams[examsTimeslots[i]].push_back(i);
        }
    } else {
        // Initialize randomizer
        std::uniform_int_distribution<int> distribution(0, timeslots - 1);

        for (int i = 0; i < exams->size(); i++) {
            // Extract random integer and fill up timeslot sample
            examsTimeslots[i] = distribution(generator);
        }
    }

}

double Solution::computePenalty() {
    double comp_penalty = 0;
    int timeslots_distance;

    // Compute penalty
    for(int i = 0; i < exams->size(); ++i){
        for(int j = i + 1; j < exams->size(); ++j){
            timeslots_distance = abs(examsTimeslots[i] - examsTimeslots[j]);
            if(
                    timeslots_distance <= 5 // difference between timeslots <= 5
                    && (*exams)[i]->hasConflict(j) // at least one student
                ) {
                comp_penalty += pow(2, (5 - timeslots_distance)) * (*exams)[i]->getConflict(j) / students;
            }
        }
    }

    return comp_penalty;
}

double Solution::getPenalty() {
    double ret_penalty;
    if(computed_penalty) {
        ret_penalty = penalty;
    } else {
        ret_penalty = penalty = computePenalty();
    }
    return ret_penalty;
}

double Solution::getGain() {
    double gain;
    double tmp_penalty = getPenalty();
    if (tmp_penalty == 0) { // avoid dividing by zero
        gain = std::numeric_limits<double>::max();
    } else {
        gain = 1.0 / tmp_penalty;
    }
    return gain;
}

void Solution::setPenalty(double new_penalty) {
    computed_penalty = true;
    penalty = new_penalty;
}
