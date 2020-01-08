#include <iostream>
#include <string>
#include <utility>

#include "data-structures/Exam.h"
#include "data-structures/Problem.h"
#include "io.h"
#include "GA/Chromosome.h"

int main(int argc, char * argv[]) {

    std::string instance_name = argv[1];
    int max_time = atoi(argv[2]);

    //int max_multistart_time = int(max_time - max_time * 0.4);

    Problem* problem = getProblemFromFile(instance_name, max_time);

    Chromosome *c1 = new Chromosome(&(problem->exams), problem->timeslots, problem->students);
    Chromosome *c2 = new Chromosome(&(problem->exams), problem->timeslots, problem->students);

    c1->crossover(c2, false);

    return 0;

}