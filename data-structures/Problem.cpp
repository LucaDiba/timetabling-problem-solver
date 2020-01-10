#include "Problem.h"
#include "../io.h"

bool Problem::handleNewSolution(Solution *newSolution) {

    // Evaluate solution
    bool isBestSolution = bestSolution == nullptr || newSolution->getGain() > bestSolution->getGain();

//    if(bestSolution != nullptr) {
//        for(int i = 0; i < newSolution->exams->size(); i++) {
//            printf("%d ", newSolution->examsTimeslots[i]);
//        }
//        printf("\n");
////        if(newSolution->isFeasible) {
////            printf("\nNew: %f Best: %f \n", newSolution->penalty, bestSolution->penalty);
////        } else {
////            printf("\nNew: unfeasible");
////        }
//    }

    if(isBestSolution) {

        // Store new best solution
        bestSolution = new Solution(
                newSolution->exams,
                newSolution->timeslots,
                newSolution->students,
                newSolution->examsTimeslots);

        bestSolution->setPenalty(newSolution->getPenalty());

        // Notify new best solution
        printf("New best solution found with penalty: %f \n", bestSolution->getPenalty());

        // Write new solution on file
        writeSolutionOnFile(this);

    }

    return isBestSolution;


};