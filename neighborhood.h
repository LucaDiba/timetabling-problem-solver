#ifndef NEIGHBORHOOD_CPP
#define NEIGHBORHOOD_CPP

#include "data-structures/Solution.h"

/**
 * Get the probability of taking the current solution
 * @param problem
 * @return probability
 */
float getProbability(Problem* problem) {
    return 0.5;
}

/**
 * Starting from a given solution, get another solution in the neighborhood
 *
 * @param current_solution the solution where to start from
 * @param iteration the iteration counter
 * @return the neighbor solution
 */
Solution getNeighbor(Solution* current_solution, int iteration) {
    Solution neighbor = *current_solution;

    // TO-DO: set new neighbor

    return neighbor;
}

/**
 * Execute Simulated Annealing algorithm on the given problem
 * @param problem
 */
void simulatedAnnealing(Problem* problem) {
    // Initial solution: problem->current_solution
    float rand_probability;
    float curr_probability;

    for(int i=0; ; ++i) {
        rand_probability = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        curr_probability = getProbability(problem);

        Solution temp_solution = getNeighbor(problem->current_solution, i);

    }
}

/**
 * Execute a neighborhood algorithm on the given problem
 * @param problem
 */
void neighborhood(Problem* problem) {
    simulatedAnnealing(problem);
}

#endif //NEIGHBORHOOD_CPP