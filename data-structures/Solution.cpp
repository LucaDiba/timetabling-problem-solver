# include "Solution.h"

/* Solution class */

Solution::Solution(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents) {

    /* Store collection length */
    exams = examsVector;
    timeslots = numberOfTimeslots;
    students = numberOfStudents;

    /* Generate collections for exams/timeslots mapping and vice-versa */
    examsTimeslots = (int*) malloc(exams->size() * sizeof(int));

    /* Initialize timeslots/exams vector */
    for(int i = 0; i < timeslots; i++)
        timeslotsExams.emplace_back(std::vector<int>());

}

bool Solution::getFeasibility() {

    /* Populate timeslots/exam vector of lists */
    for(int i = 0; i < exams->size(); i++)
        timeslotsExams[examsTimeslots[i]].push_back(i);

    /* Check feasibility */
    for(auto& timeslot: timeslotsExams){

        for(int i = 0; i < timeslot.size() && isFeasible; i++){
            for(int j = i; j < timeslot.size() && isFeasible; j++){
                if((*exams)[timeslot[i]]->hasConflict(timeslot[j])) {
                    penalty = INT_MAX;
                    isFeasible = false;
                }
            }
        }

        if(!isFeasible)
            break;

    }

    if(isFeasible)
        computePenalty();

    return isFeasible;

}

void Solution::computePenalty() {

    // Compute penalty
    for(int i = 0; i < exams->size(); i++){
        for(int j = i + 1; (j < i + 5) && (j < exams->size()) ; j++){
            if((*exams)[i]->hasConflict(j)) {
                penalty += pow(2, (5 - abs(examsTimeslots[j] - examsTimeslots[i]))) * (*exams)[i]->getConflict(j) / students;
            }
        }
    }

    // Store inverse penalty due to maximization problem
    gain = 1.0/penalty;

}

