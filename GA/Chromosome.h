#pragma once
#include <vector>
class Chromosome
{
public:
	Chromosome(int n_timeslots,int n_exams, bool initialize = true);
	~Chromosome();

	void Print();

	int** GetGenes();
	void SetGenes(int** genes);

	void Mutation();
	std::vector<Chromosome> CrossOver(Chromosome parent2);
	void Inversion();

	float CalculateFitness();

private:
	//Found in data_structure.h/Solution
	int n_timeslots,n_exams;
	int** genes;
	float fitness;

	Solution* solution;

};
