#ifndef NEIGHBORHOOD_CPP
#define NEIGHBORHOOD_CPP

#include "data-structures/Solution.h"
#include "data-structures/Problem.h"
#include <math.h> //exp
#include <random>
#include <ctime>

/**
 * Starting from a given solution, get another solution in the neighborhood
 *
 * @param current_solution the solution where to start from
 * @return the neighbor solution
 */
Solution* getNeighbor(Solution* current_solution) {
    Solution* neighbor = current_solution;

    //Execute First Improvement or pick a new random Solution
    bool found = false, FI = true;
    while (!found) {
      Solution* temp = neighbor;

      //Compiute changes:
      // Random stuff
      std::random_device device;
      std::mt19937 generator(device());
      std::uniform_int_distribution<int> examsDistribution(0, temp->exams->size());
      std::uniform_int_distribution<int> timeslotsDistribution(0, temp->timeslots);

      // Extract a random exam and a random slot to mutate
      int mutantExam = examsDistribution(generator);
      int mutantSlot = timeslotsDistribution(generator);

      // Store new timeslot for the random exam
      temp->examsTimeslots[mutantExam] = mutantSlot;

      //Check if the new solution is better the the previous one (First Improvement)
      if(FI){
        if(temp->getPenalty() > neighbor->getPenalty()){
          neighbor = temp;
          found = true;
        }
      }
      //Accept any solution (Random)
      else{
        neighbor = temp;
        found = true;
      }

    }

    return neighbor;
}

/*
 * Execute Simulated Annealing algorithm on the given problem
 * @param problem
 */
void simulatedAnnealing(Problem* problem, int max_neighborhood_time) {
    // Initial solution: problem->current_solution
    float rand_probability;

    //T_zero should be a number such that: exp( -(F(x^) - F(x~))/T_zero ) = 0.5
    //On internet, someone set T to a high number, something like 1000
    //max t=300/500 seconds.60%t to multistart remaining ~120/300s for neighborhood
    double t_zero = 10 * max_neighborhood_time;
    double T = t_zero;
    //cooling_rate = [0,1]. Should be near 1, something like 0.99
    double cooling_rate = 0.95 + max_neighborhood_time % 100;
    //(Plateau) Number of iterations after which temperature T can be changed
    int L = 5;

    Solution* best_solution;
    //Solution coming from the multistart process
    Solution* current_solution = problem->bestSolution;

    for(int i=0;;i++) {
        rand_probability = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        Solution* temp_solution = getNeighbor(current_solution);

        //Formula used to calucalate probability:
        // p^ = exp( -(F(x^) - F(x~))/T )
        //where:
        //F(x^) is the penalty of the candidate solution(the new one)
        //F(x~) is the penalty of the current solution
        //T is the 'cooling temperature'
        //F(•) is the penalty function (or similar)

        //T should change only after some steps
        if (i%L==0) {
            T = T * cooling_rate;
        }

        double prob = exp( -(temp_solution->getPenalty() - current_solution->getPenalty())/T );

        if(prob > rand_probability){
            current_solution = temp_solution;
            //Update best solution if the new solution has an higher score
            problem->handleNewSolution(best_solution);
        }

    }

}

/**
 * Execute a neighborhood algorithm on the given problem
 * @param problem
 */
void neighborhood(Problem* problem, int max_neighborhood_time) {
    simulatedAnnealing(problem, max_neighborhood_time);
}

#endif //NEIGHBORHOOD_CPP
