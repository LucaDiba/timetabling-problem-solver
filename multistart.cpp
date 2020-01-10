#include <cmath>
#include "multistart.h"

int populationSize;
float populationRatio;

//double best_fitness;
//Chromosome* best_chromosome;
//bool accept_worse_solution = false;

// Population vector
std::vector<Chromosome*> chromosomes;

void generateInitialPopulation(Problem* problem) {
//    printf("aa");
//    printf("bb");
//    printf("%f", chromosomes[0]->getFitness());

    for (int i = 0; i < populationSize; i++) {
        Chromosome* c = new Chromosome(problem, true, true);
        chromosomes.push_back(c);
        c->solution->computePenalty();
        printf("Penalty: %f\n", c->solution->penalty);
    }
//        chromosomes.push_back(new Chromosome(problem));
}

void sortPopulation(Problem* problem) {

    // Define a lambda to sort chromosomes
    std::sort(chromosomes.begin(), chromosomes.end(), [](Chromosome *one, Chromosome *two) {
        return one->getFitness() > two->getFitness();
    });

    // Evaluate new solution
    problem->handleNewSolution(chromosomes[0]->solution);

}

void evolvePopulation(Problem* problem) {

    // Sort the population in order to have easy access to the best chromosomes
    sortPopulation(problem);

    // Initialize some variables
    std::vector<Chromosome*> offspring;
    int p = int(populationSize * populationRatio);

    // There are different ways to generate a new population:
    // At the beginning you should use crossover a lot,
    // then reaching the end you should use more mutation

    // In this case I use only crossover, it works fine but when the sentence is almost complete(one or two characters are different)
    // it slows down and it takes lot of generations to find the first complete solution.

    /*  Given a population: (kind of)
            a) 1/4 are the top Chromosomes, these will not be changed
            b) 1/4 are generated with Crossover between top Chromosomes
            c) 1/4 are generated with Inversion of top Chromosomes
            d) 1/4 are generated as new Chromosomes
    */
    // TODO: add conditions for population size < 4
    int b_start = p;
    int b_stop = p * 2 - 2;

    int c_start = p * 2;
    int c_stop = p * 3 - 1;

    int d_start = p * 3;
    int d_stop = populationSize - 1;

    // (b) Crossover between top Chromosomes
    for (int j = b_start; j <= b_stop; j = j + 2) {
        // Generate offspring
        // Otherwise choose two random numbers in range [0,top_chromosomes)
        offspring = Chromosome::crossover(problem, chromosomes[j - p], chromosomes[j - p + 1]);

        // Add the new children in the population if they are better then the chromosome they will replace
        if(offspring[0]->getFitness() > chromosomes[j]->getFitness()) {
//            delete chromosomes[j];
            chromosomes[j] = offspring[0];
        }

        if(offspring[1]->getFitness() > chromosomes[j + 1]->getFitness()) {
//            delete chromosomes[j + 1];
            chromosomes[j + 1] = offspring[1];
        }

    }

    // (c) Crossover between any Chromosomes
    for (int j = c_start; j <= c_stop; j++) {
//        delete chromosomes[j];
        chromosomes[j] = chromosomes[j - p * 2]->inversion(problem);
    }

    // (d) Generate new Chromosomes
    for (int j = d_start; j <= d_stop; j++) {
//        delete chromosomes[j];
        chromosomes[j] = new Chromosome(problem);
    }

}

int computePopulationSize(Problem* problem) {

    // Source: https://www.researchgate.net/post/What_is_the_optimal_recommended_population_size_for_differential_evolution2

    // Get problem size
    int problemSize = problem->exams.size();

    // Return population size accordingly to problem size
    if (problemSize > 1000) return int(0.5 * problemSize);
    else if (problemSize > 100) return problemSize;
//    else return int(1.5 * problemSize);
    return 12; //TODO:debug
}

void multistart(Problem* problem, int maxTime, float ratio) {

    int startingTime = time(nullptr);
    int stoppingTime = startingTime + maxTime;

    // Compute population size accordingly to problem size
    populationSize = computePopulationSize(problem);
    populationRatio = ratio;

    // Generate initial population
    generateInitialPopulation(problem);


//    for (int i=0; i<chromosomes.size(); ++i) {
//        printf("a");
//        int* genes = chromosomes[i]->getGenes();
//        for (int j=0; j<4; j++) {
//            printf("%d ", genes[j]);
//        }
//        printf("\n");
//    }
//    return;

    int i=0;
    while(time(nullptr) < stoppingTime) {
        evolvePopulation(problem);
        i++;
    }

}