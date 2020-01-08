#include <algorithm>
#include <iterator>
#include <random>
#include <vector>

#include "Chromosome.h"
#include "../data-structures/Solution.h"
#include "../data-structures/Problem.h"

// The Chromosome class is a solution object(good or bad). A chromosome has a set of genes of type double array of int (size: row->n_exams, columns->n_timeslots).
// A chromosome can be mutated(one genes is changed);
// Inverted(the genes are divided in parts and these parts are reversed);
// Two chromosomes can crossover to create two new children(offspring) with genes similar to their parents.

Chromosome::Chromosome(Problem* problem, bool initializeSolution, float rate) {
    // Initialize new solution with random solution
    solution = new Solution(&(problem->exams), problem->timeslots, problem->students);
    mutationRate = rate;
    if(initializeSolution)
        solution->initializeRandomSolution();
}

Chromosome::Chromosome(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents, bool initializeSolution, float rate) {
    // Initialize new solution with random solution
    solution = new Solution(examsVector, numberOfTimeslots, numberOfStudents);
    mutationRate = rate;
    if(initializeSolution)
        solution->initializeRandomSolution();
}

Chromosome::Chromosome(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents, int *initializingSolution, float rate) {
    // Initialize new solution with given solution
    solution = new Solution(examsVector, numberOfTimeslots, numberOfStudents, initializingSolution);
    mutationRate = rate;
}

int *Chromosome::getGenes() {

    // Copy current genes
    int *genesCopy = new int[solution->exams->size()];
    std::copy(solution->examsTimeslots, solution->examsTimeslots + solution->exams->size(), genesCopy);

    return genesCopy;

}

void Chromosome::mutation() {

    // Random stuff
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<float> probabilityDistribution(0, 100);
    std::uniform_int_distribution<int> examsDistribution(0, solution->exams->size());
    std::uniform_int_distribution<int> timeslotsDistribution(0, solution->timeslots);

    // Extract a random rate which must be compared with mutation rate
    float randomProbability = probabilityDistribution(generator) / 100;

    // Randomly choose to mutate a gene
    if (randomProbability < mutationRate) {

        // Extract a random exam and a random slot to mutate
        int mutantExam = examsDistribution(generator);
        int mutantSlot = timeslotsDistribution(generator);

        // Store new timeslot for the random exam
        solution->examsTimeslots[mutantExam] = mutantSlot;

    }

}

std::vector<Chromosome> Chromosome::crossover(Chromosome *secondParent, bool ordered) {

    // Children collection
    std::vector<Chromosome> children;
    int numberOfExams = solution->exams->size();

    // Decide how many cuts will be according with the number of timeslot
    int minCut = 0, maxCut;
    int numberOfCuts = (solution->timeslots > 10) ? 2 : 1;

    // Random stuff
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> minCutDistribution(0, numberOfExams / 2 - 1);
    std::uniform_int_distribution<int> maxCutDistribution(numberOfCuts > 1 ? numberOfExams / 2 : 1, numberOfExams - 2);

    // Initialize children chromosomes
    Chromosome *firstChild = new Chromosome(
            solution->exams,
            solution->timeslots,
            solution->students,
            getGenes());

    Chromosome *secondChild = new Chromosome(
            secondParent->solution->exams,
            secondParent->solution->timeslots,
            secondParent->solution->students,
            secondParent->getGenes());

    // Extract
    maxCut = maxCutDistribution(generator);
    if (numberOfCuts == 2) {
        minCut = minCutDistribution(generator);
    }

    // Perform crossover (choose best alternative)
    if(ordered) {
        performOrderedCrossover(secondParent, firstChild, secondChild, minCut, maxCut);
    } else {
        performStandardCrossover(secondParent, firstChild, secondChild, minCut, maxCut);
    }

    children.push_back(*firstChild);
    children.push_back(*secondChild);

    return children;

}

void Chromosome::performStandardCrossover(Chromosome *secondParent, Chromosome *firstChild, Chromosome *secondChild, int minCut, int maxCut){

    // Store genes pointer to clean code
    int *firstParentGenes = solution->examsTimeslots;
    int *secondParentGenes = secondParent->solution->examsTimeslots;
    int *firstChildGenes = firstChild->solution->examsTimeslots;
    int *secondChildGenes = secondChild->solution->examsTimeslots;

    // Store exam size to perform better on iteration
    int numberOfExams = solution->exams->size();

    for(int i = 0; i < numberOfExams; i++){
        if(i < minCut || i > maxCut){

            // Swap timeslots between solutions
            firstChildGenes[i] = secondParentGenes[i];
            secondChildGenes[i] = firstParentGenes[i];

        }
    }

}

void Chromosome::performOrderedCrossover(Chromosome *secondParent, Chromosome *firstChild, Chromosome *secondChild, int minCut, int maxCut){

    //int *firstParentGenes = firstParent->GetGenes(), *secondParentGenes = secondParent->GetGenes();
    std::unordered_map<int, bool> firstParentUsedGenes, secondParentUsedGenes;

    // Store genes pointer to clean code
    int *firstParentGenes = solution->examsTimeslots;
    int *secondParentGenes = secondParent->solution->examsTimeslots;
    int *firstChildGenes = firstChild->solution->examsTimeslots;
    int *secondChildGenes = secondChild->solution->examsTimeslots;

    // Store exam size to perform better on iteration
    int numberOfExams = solution->exams->size();

    for(int i = maxCut; i < numberOfExams + minCut; i++){
        if(i % numberOfExams < minCut || i % numberOfExams > maxCut){

            // Crossover for the first chromosome
            for(int j = i; ; j = (j + 1) % numberOfExams){
                if(secondParentUsedGenes.find(j) == secondParentUsedGenes.end()) {

                    printf("%d ", j);

                    // Try to set crossover solution
                    firstChildGenes[i] = secondParentGenes[j];

                    if (firstChild->solution->getFeasibility(false)) {
                        secondParentUsedGenes[j] = true;
                        break;
                    }

                }
            }

            for(int j = i; ; j = (j + 1) % numberOfExams){
                if(firstParentUsedGenes.find(j) == firstParentUsedGenes.end()) {

                    // Try to set crossover solution
                    secondChildGenes[i] = firstParentGenes[j];

                    if (secondChild->solution->getFeasibility(false)) {
                        firstParentUsedGenes[j] = true;
                        break;
                    }

                }
            }

        }
    }

    // Perform mutation on children
    // TODO: execute mutation with probability p(fitness)
    firstChild->mutation();
    secondChild->mutation();

}

void Chromosome::inversion() {

    // Random stuff
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> cutDistribution(0, solution->exams->size()/2 - 1);

    //Decide how many cuts will be according with the number of timeslot
    int cut = cutDistribution(generator);
    int numberOfExams = solution->exams->size();

    //Copy old genes and store solution pointer
    Chromosome *invertedChromosome = new Chromosome(
            solution->exams,
            solution->timeslots,
            solution->students,
            getGenes());

    int *invertedChromosomeGenes = invertedChromosome->solution->examsTimeslots;

    // Swap genes around cutting index
    for(int i = 0; i < cut; i++)
        std::swap(invertedChromosomeGenes[i], invertedChromosomeGenes[numberOfExams - 1 - i]);

}

//The fitness is calculated as an integer between 0 and nofGenes
double Chromosome::getFitness() {

    // Compute feasibility and fitness
    solution->getFeasibility();

    return solution->gain;

}

Chromosome::~Chromosome() {

}