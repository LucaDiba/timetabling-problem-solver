#include <iostream>
#include <string>
#include <utility>

#include <random>
std::mt19937 generator;

#include "data-structures/Problem.h"
#include "io.h"
#include "multistart.h"

int main(int argc, char * argv[]) {

    std::string instance_name = argv[1];
    int max_time = atoi(argv[2]);

    int max_multistart_time = int(max_time * 0.6);
    int max_neighborhood_time = max_time - max_multistart_time;

    Problem *problem = getProblemFromFile(instance_name, max_time);
    printf("a");

    multistart(problem, max_multistart_time);
    /neighborhood(problem,max_neighborhood_time);

    return 0;
}
