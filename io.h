#ifndef IO_CPP
#define IO_CPP

#include <ctime>
#include "data_structures.h"

/* INPUT */

Problem* getProblemFromFile(std::string instance_name, int max_time) {
    Problem* p = new Problem;

    p->instance_name = std::move(instance_name);
    p->max_time = max_time;
    p->start_time = time(NULL);

    // TO-DO: complete

    return p;
}

/* OUTPUT */
void writeSolutionOnFile(Solution sol) {

}

#endif //IO_CPP