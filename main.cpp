#include <iostream>
#include <string>

#include <random>
#include <functional>

std::mt19937 generator;

#include "data-structures/Problem.h"
#include "io/io.h"
#include "multistart/multistart.h"
#include "neighborhood/neighborhood.h"

int main(int argc, char * argv[]) {

    std::string instance_name = argv[1];
    int max_time = atoi(argv[3]);
    printf("max time: %d\n", max_time);


    int max_multistart_time = int(max_time * 0.8);

    Problem *problem = getProblemFromFile(instance_name, max_time);

    printf("Multistart\n");
    multistart(problem, max_multistart_time);

    int elapsed_time = (time(nullptr) - problem->start_time);
    int max_neighborhood_time = problem->max_time - elapsed_time;

    printf("Neighborhood\n");
    neighborhood(problem, max_neighborhood_time);

    return 0;
}
