#include "Chromosome.h"
#include <iostream>
#include <stdlib.h>
#include<ctime>
#include "data_structures.h"
using namespace std;

//The Chromosome class is solution (good or bad). A chromosome has a set of genes of type double array of int (size: row->n_exams, columns->n_timeslots).
//A chromosome can be mutated(one genes if changed) or inverted(the genes are divided in parts and these parts are reversed)
//Two chromosomes can crossover to create two new children(offspring) with genes similar to their parents

Chromosome::Chromosome(int n_exams,int n_timeslots, bool initialize)
{
	Chromosome::n_exams = n_exams;
	Chromosome::n_timeslots = n_timeslots;
	Solution* solution;

	if (initialize)
	{
		//Create space to store variables
      solution->exams_timeslot = new int[n_timeslots];
      solution->exams_timeslot_matrix = new int*[n_exams];
      for (int i = 0; i < n_exams; i++) {
        solution->exams_timeslot_matrix[i] = new int[n_timeslots];
      }

      //Initialize
      for (int i = 0; i < n_exams; i++) {
        for (int j = 0; j < n_timeslots; j++) {
          //It could be used a better method.
          //It may happen that an exam is not scheduled(rand() % 100 > 50 always false)
          if (rand() % 100 > 50) {
            solution->exams_timeslot_matrix[i][j] = 1;
            //For feasibility: exit after the exams is scheduled on a time slot
            j = problem->n_timeslots;
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
			g[i][j] = genes[i][j];
		}
	}
	return g;
}

void Chromosome::SetGenes(int** genes)
{
	Chromosome::genes = genes;
}

void Chromosome::Mutation()
{
	//TO DO
}

vector<Chromosome> Chromosome::CrossOver(Chromosome parent2)
{
	//TO DO
	/*
	vector<Chromosome> children;
	Chromosome* c1 = new Chromosome(nofGenes,word, false);
	Chromosome* c2 = new Chromosome(nofGenes,word, false);

	//Decide how many cuts will be according with the sentence length
	int nofCuts = 1;
	if (nofGenes > 10)
		nofCuts = 2;

	int cutMin, cutMax = 0;
	if (nofCuts == 2)
	{
		cutMin = 1 + rand() % (nofGenes / 2 - 1);
		cutMax = cutMin + 1 + rand() % (nofGenes / 2 - 2);
	}
	else {
		cutMin = 1 + rand() % (nofGenes - 1);
	}

	char* newGenes = new char[nofGenes];

	//Child 1
	newGenes = GetGenes();
	for (int i = cutMin; i < nofGenes - cutMax; i++) {
		newGenes[i] = parent2.GetGenes()[i];
	}

	c1->SetGenes(newGenes);
	c1->Mutation();
	children.push_back(*c1);

	//Child 2
	newGenes = parent2.GetGenes();
	for (int i = cutMin; i < nofGenes - cutMax; i++)
		newGenes[i] = genes[i];
	c2->SetGenes(newGenes);
	c2->Mutation();
	children.push_back(*c2);

	return children;
	*/
}

void Chromosome::Inversion()
{
	//TO DO
	int nofCuts = 1;
	if (nofGenes > 10)
		nofCuts = 2;

	int cutMin, cutMax = 0;
	if (nofCuts == 2)
	{
		cutMin = 1 + rand() % (nofGenes / 2 - 1);
		cutMax = cutMin + 1 + rand() % (nofGenes / 2 - 2);
	}
	else {
		cutMin = 1 + rand() % (nofGenes - 1);
	}

	//Copy old genes
	char* chars = new char[nofGenes];
	for (int i = 0; i < nofGenes; i++)
		chars[i] = genes[i];


	if (nofCuts == 1)
	{
		for (int i = 0; i < nofGenes - (cutMin + cutMax); i++)
			genes[i] = chars[i + cutMin + cutMax];
		for (int i = nofGenes - (cutMin + cutMax); i < nofGenes; i++)
			genes[i] = chars[i - (nofGenes - (cutMin + cutMax))];
	}
	else if (nofCuts == 2) {
		//Character after cutMax are placed at the beginning of new genes
		for (int i = cutMax; i < nofGenes; i++)
			genes[i - cutMax] = chars[i];
		//Character between cutMin and cutMax are not moved
		for (int i = cutMin; i < cutMax; i++)
			genes[i + nofGenes - cutMax - 2] = chars[i];
		//Character before cutMin are placed at the end of new genes
		for (int i = 0; i < cutMin; i++)
			genes[i + nofGenes - cutMin] = chars[i];
	}

	delete[] chars;
}

//The fitness is calculated as an integer between 0 and nofGenes
float Chromosome::CalculateFitness()
{
	fitness = 0;
	//TO DO
	return fitness;
}

Chromosome::~Chromosome()
{
}
