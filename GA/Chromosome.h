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
	Chromosome* CrossOverHelper();
	void Inversion();

	float CalculateFitness();

private:
	int n_timeslots,n_exams;
	float mutation_rate;
	//Found in data_structure.h/Solution
	int** genes;
	float fitness;

	Solution* solution;

};
