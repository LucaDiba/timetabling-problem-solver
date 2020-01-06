#include "Chromosome.h"
#include <iostream>
#include <stdlib.h>
#include<ctime>
#include "data_structures.h"
using namespace std;

//The Chromosome class is a solution object(good or bad). A chromosome has a set of genes of type double array of int (size: row->n_exams, columns->n_timeslots).
//A chromosome can be mutated(one genes is changed);
//Inverted(the genes are divided in parts and these parts are reversed);
//Two chromosomes can crossover to create two new children(offspring) with genes similar to their parents.

Chromosome::Chromosome(int n_exams,int n_timeslots, bool initialize)
{
	n_exams = n_exams;
	n_timeslots = n_timeslots;
	mutation_rate = 0.1f;

	//Create space to store variables
	solution->exams_timeslot = new int[n_timeslots];
	solution->exams_timeslot_matrix = new int*[n_exams];
	for (int i = 0; i < n_exams; i++) {
		solution->exams_timeslot_matrix[i] = new int[n_timeslots];
	}

	if (initialize)
	{
      //Initialize 2d array
      for (int i = 0; i < n_exams; i++) {
        for (int j = 0; j < n_timeslots; j++) {
          //It could be used a better method.
          //It may happen that an exam is not scheduled(rand() % 100 > 50 always false)
          if (rand() % 100 > 50) {
            solution->exams_timeslot_matrix[i][j] = 1;
            //For feasibility: exit after the exams is scheduled on a time slot
            j = n_timeslots;
          }
        }
      }

		}
}

//Returns a copy of the current genes
int** Chromosome::GetGenes()
{
	int** g = new int*[n_exams];
	for (int i = 0; i < n_exams; i++) {
		g[i] = new int[n_timeslots];
	}
	for (int i = 0; i < n_exams; i++) {
		for (int j = 0; j < n_timeslots; j++) {
			g[i][j] = solution->exams_timeslot_matrix[i][j];
		}
	}
	return g;
}

void Chromosome::SetGenes(int** genes)
{
	solution->exams_timeslot_matrix = genes;
}

void Chromosome::Mutation()
{
	float r = (rand() % 100) / 100;

	if (r<mutation_rate) {
		int i = rand() % n_exams;
		//Comment the for loop to mutate only 1 row of the 2d array
		//Otherwise it will flip an element in each row
		for (i = 0; i < n_exams; i++)
		{
			//Choose a random number [0,n_timeslots-1]. This will be the index of element to be flipped
			int index = rand() % n_timeslots;
			//If the element at position index is already 1 choose another element
			if (solution->exams_timeslot_matrix[i][index]==1) {
				int index = rand() % n_timeslots;
			}
			//Set to 0 all the elements in the current row
			for (int j = 0; j < n_timeslots; j++) {
				if (solution->exams_timeslot_matrix[i][j]==1) {
					solution->exams_timeslot_matrix[i][j]=0;
					//Exit after finding the element with 1
					//since there is only a 1 in each row
					j=n_timeslots;
				}
			}
			//Set the element at pos index to 1
			solution->exams_timeslot_matrix[i][index] = 1;
		}
	}
}

vector<Chromosome> Chromosome::CrossOver(Chromosome parent2)
{
	vector<Chromosome> children;

	//Decide how many cuts will be according with the number of timeslot
	int nofCuts = 1;
	if (n_timeslots > 10)
		nofCuts = 2;

	int cutMin, cutMax = 0;
	if (nofCuts == 2)
	{
		cutMin = 1 + rand() % (n_exams / 2 - 1);
		cutMax = cutMin + 1 + rand() % (n_exams / 2 - 2);
	}
	else {
		cutMin = 1 + rand() % (n_exams - 1);
	}

	//Child 1
	children.push_back( *CrossOverHelper( parent2.GetGenes(),GetGenes() ) );

	//Child 2
	children.push_back( *CrossOverHelper( GetGenes(),parent2.GetGenes() ) );

	return children;
}

void Chromosome::CrossOverHelper(int** parent1_genes, int** parent2_genes){
	Chromosome* c = new Chromosome(n_timeslots,n_exams,false);

	for (int i = cutMin; i < n_exams - cutMax; i++){
		for (int j = 0; j < n_timeslots; j++)
			parent1_genes[i][j] = parent2_genes[i][j];
	}

	c->SetGenes(parent1_genes);
	c->Mutation();

	return c;
}

void Chromosome::Inversion()
{
	int nofCuts = 1;
	if (n_timeslots > 10)
		nofCuts = 2;

	int cutMin, cutMax = 0;
	if (nofCuts == 2)
	{
		cutMin = 1 + rand() % (n_timeslots / 2 - 1);
		cutMax = cutMin + 1 + rand() % (n_timeslots / 2 - 2);
	}
	else {
		cutMin = 1 + rand() % (n_timeslots - 1);
	}

	//Copy old genes
	int** old_genes = GetGenes();


	if (nofCuts == 1)
	{
		//Loop for each row
		for (int r = 0; r < n_exams; r++) {
			for (int i = 0; i < n_timeslots - cutMin; i++)
				genes[r][i] = old_genes[r][i + cutMin];
			for (int i = n_timeslots - cutMin; i < n_timeslots; i++)
				genes[r][i] = old_genes[r][i - (n_timeslots - cutMin)];
		}
	}
	else if (nofCuts == 2) {
		for (int r = 0; r < n_exams; r++) {
			//Elements after cutMax are placed at the beginning of the new genes
			for (int i = cutMax; i < nofGenes; i++)
				genes[r][i - cutMax] = old_genes[r][i];
			//Elements between cutMin and cutMax are not moved
			for (int i = cutMin; i < cutMax; i++)
				genes[r][i + nofGenes - cutMax - 2] = old_genes[r][i];
			//Elements before cutMin are placed at the end of the new genes
			for (int i = 0; i < cutMin; i++)
				genes[r][i + nofGenes - cutMin] = old_genes[r][i];
		}
	}

	//Free memeory: old genes
	// delete[] chars;
}

//The fitness is calculated as an integer between 0 and nofGenes
float Chromosome::CalculateFitness()
{
	fitness = 0;
	//TO DO
	solution->penalty = fitness;
	return fitness;
}

Chromosome::~Chromosome()
{
}
