#ifndef MULTISTART_CPP
#define MULTISTART_CPP

#include <ctime>
#include "data_structures.h"

/**
 * Set the initial solution on the given problem
 * @param problem
 */
void setInitialSolution(Problem* problem) {
    // TO-DO:
}

/**
 * Execute a multi-start algorithm on the given problem
 * @param problem
 */
void multistart(Problem* problem, int max_multistart_time) {
    int starting_time = time(NULL);
    int stopping_time = starting_time + max_multistart_time;

    setInitialSolution(problem);

    // TO-DO: multi-start algorithm

}

#endif //MULTISTART_CPP