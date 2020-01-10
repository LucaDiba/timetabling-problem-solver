#include "Problem.h"
#include "../io.h"

bool Problem::handleNewSolution(Solution *newSolution) {

    // Evaluate solution
    bool isBestSolution = bestSolution == nullptr || newSolution->getPenalty() < bestSolution->getPenalty();

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