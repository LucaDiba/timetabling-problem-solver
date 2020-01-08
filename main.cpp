#include <iostream>
#include <string>
#include <utility>

#include "data-structures/Exam.h"
#include "data-structures/Problem.h"
#include "io.h"

int main(int argc, char * argv[]) {

    std::string instance_name = argv[1];
    int max_time = atoi(argv[2]);

    //int max_multistart_time = int(max_time - max_time * 0.4);

    Problem* problem = getProblemFromFile(instance_name, max_time);
    problem->exams[0]->printAllConflicts(&problem->exams, true);

    return 0;

}