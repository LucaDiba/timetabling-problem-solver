#include <cmath>
#include "multistart.h"
#include "data-structures/rand.h"

int population_size;

//double best_fitness;
//Chromosome* best_chromosome;
//bool accept_worse_solution = false;

// Population vector
std::vector<Chromosome*> chromosomes;

void generateInitialPopulation(Problem* problem) {

    /* Statistics on generated solutions */ //TODO: remove for production for better performances
    double tmp_penalty;
    double min_penalty = std::numeric_limits<double>::max();
    double max_penalty = std::numeric_limits<double>::min();
    double sum_penalty = 0;

    for (int i = 0; i < populationSize; i++) {

        Chromosome* c = new Chromosome(problem);
        chromosomes.push_back(c);

        tmp_penalty = c->solution->getPenalty();
        min_penalty = (tmp_penalty < min_penalty) ? tmp_penalty : min_penalty;
        max_penalty = (tmp_penalty > max_penalty) ? tmp_penalty : max_penalty;
        sum_penalty += tmp_penalty;

    }

    printf("Random starting solution statistics:\n");
    printf("- Max penalty:  %f\n", max_penalty);
    printf("- Min penalty:  %f\n", min_penalty);
    printf("- Mean penalty: %f\n", sum_penalty / population_size);
}

void sortPopulation(Problem* problem) {
    printf(">> start sort\n");

    // Define a lambda to sort chromosomes
    std::sort(chromosomes.begin(), chromosomes.end(), [](Chromosome *one, Chromosome *two) {
        return one->getFitness() > two->getFitness();
    });

    printf(">> end sort\n");

    // Evaluate new solution
    problem->handleNewSolution(chromosomes[0]->solution);

}

void evolvePopulation(Problem* problem) {

    // Sort the population in order to have easy access to the best chromosomes
    sortPopulation(problem);

    // Initialize some variables
    std::vector<Chromosome*> offspring;

    // There are different ways to generate a new population:
    // At the beginning you should use crossover a lot,
    // then reaching the end you should use more mutation

    // In this case I use only crossover, it works fine but when the sentence is almost complete(one or two characters are different)
    // it slows down and it takes lot of generations to find the first complete solution.

    /*  Given a population: (kind of)
            a) 5% are the top Chromosomes, these will not be changed - elite - http://www.zemris.fer.hr/~golub/clanci/iti2004.pdf - https://www.mathworks.com/help/gads/how-the-genetic-algorithm-works.html
            b) 20% (1/4 - 5%) are generated with Crossover between Chromosomes
            c) 25% (1/4) are generated with Inversion of top Chromosomes
            d) 25% (1/4) are generated as new Chromosomes
    */

    int a_start = 0;
    int a_stop = std::max(1, int(population_size * 0.05));

    int b_start = a_stop;
    int b_stop = b_start + int(population_size * 0.25) - a_stop;

    int c_start = b_stop;
    int c_stop = b_start + int(population_size * 0.25);

    int d_start = c_stop;
    int d_stop = population_size;

    // (b) Crossover between top Chromosomes
    for (int j = b_start; j <= b_stop; j = j + 2) {

        // Generate offspring
        // Otherwise choose two random numbers in range [0,top_chromosomes)
        offspring = Chromosome::crossover(problem, chromosomes[random_parent_1], chromosomes[random_parent_2]);

        // Add the new children in the population if they are better then the chromosome they will replace
        if(offspring[0]->getFitness() > chromosomes[j]->getFitness()) {
            delete chromosomes[j];
            chromosomes[j] = offspring[0];
        }

        if(offspring[1]->getFitness() > chromosomes[j + 1]->getFitness()) {
            delete chromosomes[j + 1];
            chromosomes[j + 1] = offspring[1];
        }

    }

    // (c) Inversion of top Chromosomes
    std::uniform_int_distribution<int> elite_distribution(0, a_stop - 1);
    for (int j = c_start; j < c_stop; ++j) {
        int random_elite = elite_distribution(generator);

        delete chromosomes[j];
        chromosomes[j] = chromosomes[random_elite]->inversion(problem);


        printf("inversion feasibility: %d\n", chromosomes[j]->solution->getFeasibility());
    }

    // (d) Generate new Chromosomes
    for (int j = d_start; j < d_stop; ++j) {
        delete chromosomes[j];
        chromosomes[j] = new Chromosome(problem);
    }

}

int computePopulationSize(Problem* problem) {

    // Source: https://www.researchgate.net/post/What_is_the_optimal_recommended_population_size_for_differential_evolution2

    // Get problem size
    int problemSize = problem->exams.size();

    // TODO: Compute population size according to problem size or density
    return 12;

    // Return population size accordingly to problem size
    if (problemSize > 1000) return int(0.5 * problemSize);
    else if (problemSize > 100) return problemSize;
    else return int(1.5 * problemSize);

}

void multistart(Problem* problem, int maxTime) {

    int startingTime = time(nullptr);
    int stoppingTime = startingTime + maxTime;

    // Compute population size accordingly to problem size
    population_size = computePopulationSize(problem);

    // Generate initial population
    generateInitialPopulation(problem);

    int i=0;
    while(time(nullptr) < stoppingTime) {
        evolvePopulation(problem);
        printf("Generation %d: \n", ++i);
    }

}
