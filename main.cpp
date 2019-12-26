#include <iostream>
#include <string>
#include <utility>
#include <ctime>

#include "data_structures.cpp"
#include "io.cpp"
#include "multistart.cpp"
#include "neighborhood.cpp"

int main(int argc, char * argv[]) {
    std::string instance_name = argv[1];
    int max_time = atoi(argv[3]);

    Problem* problem = getProblemFromFile(instance_name, max_time);

    multistart(problem);
    neighborhood(problem);

    return 0;
}