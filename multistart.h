#ifndef MULTISTART_CPP
#define MULTISTART_CPP

#include <ctime>
#include "data_structures.h"

#include <vector>     //vector
#include <stdlib.h>   //random
#include <algorithm>  //sort
#include "GA/Chromosome.h"
std::vector<Chromosome> chromosomes;
int populationSize;
int bestFitness;
Chromosome* bestChromosome;
bool acceptWorseSolution = false;
int topChromosomes = population * 0.2;
/**
 * Set the initial solution on the given problem
 * @param problem
 */
void setInitialSolution(Problem* problem) {
    // TO-DO:
    for (int i = 0; i < populationSize; i++) {
      Chromosome* c = new Chromosome(problem->n_timeslots,problem->n_exams);
      c->Mutation();
      chromosomes.push_back(*c);
    }
}

/**
 * Execute a multi-start algorithm on the given problem
 * @param problem
 */
void multistart(Problem* problem, int max_multistart_time) {
    int starting_time = time(NULL);
    int stopping_time = starting_time + max_multistart_time;

    setInitialSolution(problem);

    // TO-DO: multi-start algorithm

}

//Sort the population according to the increasing chromosomes' fitness
void SortPopulation()
{
	std::sort(chromosomes.begin(), chromosomes.end(), [](Chromosome & one, Chromosome & two) {return one.CalculateFitness() > two.CalculateFitness(); });
	if (chromosomes[0].CalculateFitness()>bestFitness)
	{
		bestFitness = chromosomes[0].CalculateFitness();
		bestChromosome = chromosomes[0].GetGenes();
	}
}

//Create a new population using the current population
void CreateNewPopulation(Problem* problem)
{
	//Sort the population in order to have easy access to the best chromosomes
	SortPopulation();

	//Initialize some variables
	Chromosome* c1 = new Chromosome(problem->n_timeslots,problem->n_exams,false);
	Chromosome* c2 = new Chromosome(problem->n_timeslots,problem->n_exams,false);
	vector<Chromosome> offsping;

	int n1, n2, counter = 0;

	//There are different ways to generate a new population:
	//At the beginning you should use crossover a lot,
	//then reaching the end you should use more mutation

	//In this case I use only crossover, it works fine but when the sentence is almost complete(one or two characters are different)
	//it slows down and it takes lot of generations to find the first complete solution.

	/*Given a population: (kind of)
	- 1/5 are the top Chromosomes, these will not be changed,
	- 1/5 are generated with Crossover between top Chromosomes
	- 1/5 are generated with Crossover between any Chromosomes
	- 1/5 are generated with Inversion of top Chromosomes
	- 1/5 are generated as new Chromosomes
	*/

	//Crossover between top Chromosomes
	for (int j = 0; j < topChromosomes / 2 * 4; j++)
	{
		//Choose two random numbers in range [0,topChromosomes)
		n1 = rand() % (topChromosomes);
		n2 = rand() % (topChromosomes);
		//Generate new offspring
		offsping = chromosomes[n1].CrossOver(chromosomes[n2]);
		c1 = &(offsping.at(0));
		c2 = &(offsping.at(1));
		//Add the new children in the population if they are better then the chromosome the will be replaced
		if (acceptWorseSolution || chromosomes.at(topChromosomes + counter).CalculateFitness() <= c1->CalculateFitness()) {
			chromosomes.at(topChromosomes + counter) = *c1;
		}
		counter++;

		if (acceptWorseSolution || chromosomes.at(topChromosomes + counter).CalculateFitness() <= c2->CalculateFitness()){
			chromosomes.at(topChromosomes + counter) = *c2;
		}

		counter++;
	}

	//Crossover between any Chromosomes
	/*for (int j = 0; j < topChromosomes/2; j++)
	{
		n1 = rand() % (populationSize);
		n2 = rand() % (populationSize);
		offsping = chromosomes[n1].CrossOver(chromosomes[n2]);
		c1 = &(offsping.at(0));
		c2 = &(offsping.at(1));

		if (acceptWorseSolution || chromosomes.at(topChromosomes + counter).CalculateFitness() <= c1->CalculateFitness()) {
			chromosomes.at(topChromosomes + counter) = *c1;
		}
		counter++;

		if (acceptWorseSolution || chromosomes.at(topChromosomes + counter).CalculateFitness() <= c2->CalculateFitness()){
			chromosomes.at(topChromosomes + counter) = *c2;
		}
	}*/

	//Inversion
	/*for (int j = 0; j < topChromosomes / 2; j++)
	{
		n1 = rand() % (topChromosomes);

		c1 = &chromosomes.at(n1);
		c1->Inversion();

		if (acceptWorseSolution || chromosomes.at(topChromosomes + counter).CalculateFitness() <= c1->CalculateFitness()) {
			chromosomes.at(topChromosomes + counter) = *c1;
		}
		counter++;
	}*/
}

#endif //MULTISTART_CPP
