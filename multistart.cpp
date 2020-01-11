#include <cmath>
#include "multistart.h"
#include "data-structures/rand.h"

int populationSize;

// Population vector
std::vector<Chromosome*> chromosomes;

void generateInitialPopulation(Problem* problem) {

    /* Statistics on generated solutions */ //TODO: remove for production for better performances
    double tmp_penalty;
    double min_penalty = std::numeric_limits<double>::max();
    double max_penalty = std::numeric_limits<double>::min();
    double sum_penalty = 0;

    for (int i = 0; i < populationSize; i++) {

        Chromosome* c = new Chromosome(problem, true);
        chromosomes.push_back(c);

        tmp_penalty = c->solution->getPenalty();
        min_penalty = (tmp_penalty < min_penalty) ? tmp_penalty : min_penalty;
        max_penalty = (tmp_penalty > max_penalty) ? tmp_penalty : max_penalty;
        sum_penalty += tmp_penalty;

    }

    printf("Random starting solution statistics:\n");
    printf("- Max penalty:  %f\n", max_penalty);
    printf("- Min penalty:  %f\n", min_penalty);
    printf("- Mean penalty: %f\n", sum_penalty / populationSize);
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
    std::vector<Chromosome*> previous_generation_chromosomes = chromosomes;

    // There are different ways to generate a new population:
    // At the beginning you should use crossover a lot,
    // then reaching the end you should use more mutation

    // In this case I use only crossover, it works fine but when the sentence is almost complete(one or two characters are different)
    // it slows down and it takes lot of generations to find the first complete solution.

    /*  Given a population: (kind of)
            a) max(1, 5%) are the top Chromosomes, these will not be changed - elite - http://www.zemris.fer.hr/~golub/clanci/iti2004.pdf - https://www.mathworks.com/help/gads/how-the-genetic-algorithm-works.html
            b) 20% (1/4 - 5%) are generated with Crossover between Chromosomes
            c) 25% (1/4) are generated with Inversion of top Chromosomes
            d) 25% (1/4) are generated as new Chromosomes
    */

    int a_start = 0;
    int a_stop = std::max(1, int(populationSize * 0.05));

    int b_start = a_stop;
    int b_stop = b_start + int(populationSize * 0.25) - a_stop;

    int c_start = b_stop;
    int c_stop = b_start + int(populationSize * 0.25);

    int d_start = c_stop;
    int d_stop = populationSize;

    // Elite distribution
    std::uniform_int_distribution<int> elite_distribution(0, a_stop - 1);

    // (b) Crossover between Chromosomes
    for (int j = b_start; j < b_stop - 1; j = j + 2) {
        /* Take a random chromosome
         * If it's an already substituted chromosome, take it from previous generation to maintain uniformity
         */
        std::uniform_int_distribution<int> population_distribution(0, populationSize - 1);

        int random_parent_1_id = population_distribution(generator);
        int random_parent_2_id = population_distribution(generator);
        Chromosome* random_parent_1 = previous_generation_chromosomes[random_parent_1_id];
        Chromosome* random_parent_2 = previous_generation_chromosomes[random_parent_2_id];

        // Generate offsprings
        offspring = Chromosome::crossover(problem, random_parent_1, random_parent_2);

        // Add the new children in the population if they are better then the chromosome they will replace
        if(offspring[0]->getFitness() > chromosomes[j]->getFitness()) {
            chromosomes[j] = offspring[0];
        }

        if(offspring[1]->getFitness() > chromosomes[j + 1]->getFitness()) {
            chromosomes[j + 1] = offspring[1];
        }
    }

    // (c) Inversion of top Chromosomes
    for (int j = c_start; j < c_stop; ++j) {
        int random_elite_id = elite_distribution(generator);
        chromosomes[j] = chromosomes[random_elite_id]->inversion(problem);
    }

    // (d) Generate new Chromosomes
    for (int j = d_start; j < d_stop; ++j) {
        chromosomes[j] = new Chromosome(problem);
    }

    /* Free memory from previous generation chromosomes */
    for(Chromosome* c: previous_generation_chromosomes) {
//        delete c; //TODO: delete only replaced chromosomes to avoid nullptr exception
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
    else return std::max(5, int(1.5 * problemSize));
}

void multistart(Problem* problem, int maxTime) {

    int startingTime = time(nullptr);
    int stoppingTime = startingTime + maxTime;

    // Compute population size accordingly to problem size
    populationSize = computePopulationSize(problem);

    // Generate initial population
    generateInitialPopulation(problem);

    int i=0;
    while(time(nullptr) < stoppingTime) {
        ++i;
        if(i % 100 == 0)
            printf("Generation %d\n", i);
        evolvePopulation(problem);
    }

}
