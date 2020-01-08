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

    Chromosome *c1 = new Chromosome(&(problem->exams), problem->timeslots, problem->students);
    Chromosome *c2 = new Chromosome(&(problem->exams), problem->timeslots, problem->students);

    return 0;

}