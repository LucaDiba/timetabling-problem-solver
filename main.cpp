#include <iostream>
#include <string>
#include <utility>

#include "data_structures.h"
#include "io.h"
#include "multistart.h"
#include "neighborhood.h"

int main(int argc, char * argv[]) {
    std::string instance_name = argv[1];
    int max_time = atoi(argv[3]);
    int max_multistart_time = int(max_time - max_time * 0.4);

    Problem* problem = getProblemFromFile(instance_name, max_time);

    multistart(problem, max_multistart_time);
    neighborhood(problem);

    return 0;
}