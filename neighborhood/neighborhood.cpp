#include "neighborhood.h"
#include "../data-structures/rand.h"

void neighborhood(Problem* problem, int max_neighborhood_time) {
    simulatedAnnealing(problem, max_neighborhood_time);
}

Solution* getNeighbor(Problem* problem) {
    Solution* neighbor = problem->currentSolution;

    std::uniform_int_distribution<int> examsDistribution(0, problem->exams.size());
    std::uniform_int_distribution<int> timeslotsDistribution(0, problem->timeslots);

    bool found = false;
    while (!found) {
        Solution* temp = neighbor;

        // Extract a random exam and a random slot to mutate
        int mutant_exam = examsDistribution(generator);
        int current_slot = neighbor->examsTimeslots[mutant_exam];
        int mutant_slot = (current_slot + int(problem->timeslots / 2)) % problem->timeslots;
        
        temp->moveExam((*temp->exams)[mutant_exam], mutant_slot);

        if(temp->getPenalty() > neighbor->getPenalty()){
            neighbor = temp;
            found = true;

            printf("%f\n", temp->getPenalty());
            printf("%f\n", neighbor->getPenalty());
            printf("%f\n", problem->currentSolution->getPenalty());
            printf("%f\n", problem->bestSolution->getPenalty());
            exit(0);
        }

    }

    return neighbor;
}

double getCurrentProbability(Problem* problem, Solution* sol, double T) {
    return exp( -(sol->getPenalty() - problem->currentSolution->getPenalty()) / T );
}

void simulatedAnnealing(Problem* problem, int max_neighborhood_time) {
    // Initial solution: problem->current_solution
    std::uniform_int_distribution<int> percent_distribution(0, 100);
    float rand_probability;

    /* T_zero should be a number such that: exp( -(F(x^) - F(x~))/T_zero ) = 0.5
     * On internet, someone set T to a high number, something like 1000
     * max t=180/300 seconds. 60%t to multistart, remaining ~108/180s for neighborhood
     */
    double t_zero = 5 * max_neighborhood_time;

    double T = t_zero;

    /* cooling_rate = [0,1]. Should be near 1, something like 0.99
     * set it to a smaller value will speed up the process but it may skip best solutions
     */
    double cooling_rate = 0.80 + max_neighborhood_time % 10;

    /* (Plateau) Number of iterations after which temperature T can be changed */
    int L = 5;

    problem->currentSolution = problem->bestSolution; // copy the entire solution if you delete it after

    for(int i = 0; ; ++i) {
        rand_probability = float(percent_distribution(generator)) / 100;

        Solution* tempSolution = getNeighbor(problem);

        //Formula used to calucalate probability:
        // p^ = exp( -(F(x^) - F(x~))/T )
        //where:
        //F(x^) is the penalty of the candidate solution(the new one)
        //F(x~) is the penalty of the current solution
        //T is the 'cooling temperature'
        //F(•) is the penalty function (or similar)

        //T should change only after some steps
        if (i % L == 0) {
            T = T * cooling_rate;
        }

        double prob = getCurrentProbability(problem, tempSolution, T);

        if (prob > rand_probability) {
            problem->currentSolution = tempSolution;
            problem->handleNewSolution(problem->currentSolution);
        } else {
            delete tempSolution;
        }

    }

}