#include <algorithm>
#include <iterator>
#include <vector>

#include "Chromosome.h"
#include "../data-structures/Solution.h"
#include "../data-structures/Problem.h"
#include "../data-structures/rand.h"

float mutRate = 1;

Chromosome::Chromosome(Problem* problem) {
    mutationRate = mutRate;

    solution = new Solution(&(problem->exams), problem->timeslots, problem->students);
    solution->initializeRandomSolution(true);
}

Chromosome::Chromosome(Problem* problem, int *initializingSolution) {
    mutationRate = mutRate;

    solution = new Solution(&problem->exams, problem->timeslots, problem->students, initializingSolution);
}

int *Chromosome::getGenes() {
    // Copy current genes
    int *genesCopy = new int[solution->exams->size()];
    std::copy(solution->examsTimeslots, solution->examsTimeslots + solution->exams->size(), genesCopy);

    return genesCopy;

}

void Chromosome::mutation() {

    // Random stuff
    std::uniform_int_distribution<int> probabilityDistribution(0, 100);
    std::uniform_int_distribution<int> examsDistribution(0, solution->exams->size() - 1);
    std::uniform_int_distribution<int> timeslotsDistribution(0, solution->timeslots - 1);

    // Extract a random rate which must be compared with mutation rate
    float randomProbability = float(probabilityDistribution(generator)) / 100.0;

    // Randomly choose to mutate a gene
    if (randomProbability < mutationRate) {

        // Extract a random exam and a random slot to mutate
        int mutantExam = examsDistribution(generator);
        int mutantSlot = timeslotsDistribution(generator);

        // Store new timeslot for the random exam
        solution->examsTimeslots[mutantExam] = mutantSlot;

    }

}

void performStandardCrossover(Chromosome *firstParent, Chromosome *secondParent, Chromosome *firstChild, Chromosome *secondChild, int minCut, int maxCut){

    // Store genes pointer to clean code
    int *firstParentGenes = firstParent->solution->examsTimeslots;
    int *secondParentGenes = secondParent->solution->examsTimeslots;
    int *firstChildGenes = firstChild->solution->examsTimeslots;
    int *secondChildGenes = secondChild->solution->examsTimeslots;

    // Store exam size to perform better on iteration
    int numberOfExams = firstParent->solution->exams->size();

    for(int i = 0; i < numberOfExams; i++){
        if(i < minCut || i > maxCut){

            // Swap timeslots between solutions
            firstChildGenes[i] = secondParentGenes[i];
            secondChildGenes[i] = firstParentGenes[i];

        }
    }

    // Perform mutation on children
    firstChild->mutation();
    secondChild->mutation();

}

void performOrderedCrossover(Chromosome *firstParent, Chromosome *secondParent, Chromosome *firstChild, Chromosome *secondChild, int minCut, int maxCut, bool *performingCrossover){

    std::unordered_map<int, bool> firstParentUsedGenes, secondParentUsedGenes;

    // Store genes pointer to clean code
    int *firstParentGenes = firstParent->solution->examsTimeslots;
    int *secondParentGenes = secondParent->solution->examsTimeslots;
    int *firstChildGenes = firstChild->solution->examsTimeslots;
    int *secondChildGenes = secondChild->solution->examsTimeslots;

    // Store exam size to perform better on iteration
    int numberOfExams = firstParent->solution->exams->size();

    for(int i = maxCut; i < numberOfExams + minCut + 1; i++){
        if(i % numberOfExams < minCut || i % numberOfExams > maxCut){

            // Crossover for the first chromosome
            for(int j = i % numberOfExams, k = 0; *performingCrossover; j = (j + 1) % numberOfExams, k++){
                if(secondParentUsedGenes.find(j) == secondParentUsedGenes.end()) {

                    if(k >= maxCut - minCut + 1) {
                        *performingCrossover = false;
                        break;
                    }

                    // Try to set crossover solution
                    firstChildGenes[i] = secondParentGenes[j];
                    if (firstChild->solution->getFeasibility(false)) {
                        secondParentUsedGenes[j] = true;
                        break;
                    }

                }
            }

            for(int j = i % numberOfExams, k = 0; *performingCrossover; j = (j + 1) % numberOfExams, k++){
                if(firstParentUsedGenes.find(j) == firstParentUsedGenes.end()) {

                    if(k >= maxCut - minCut + 1) {
                        *performingCrossover = false;
                        break;
                    }

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

std::vector<Chromosome*> Chromosome::crossover(Problem* problem, Chromosome *firstParent, Chromosome *secondParent, bool ordered) {

    // Children collection
    std::vector<Chromosome*> children;
    int numberOfExams = firstParent->solution->exams->size();

    // Decide how many cuts will be according with the number of timeslot
    int minCut = 0, maxCut;
    int numberOfCuts = (firstParent->solution->timeslots > 10) ? 2 : 1;

    // Random stuff
    std::uniform_int_distribution<int> minCutDistribution(0, numberOfExams/2 - 1);
    std::uniform_int_distribution<int> maxCutDistribution(numberOfCuts > 1 ? numberOfExams/2 : 1, numberOfExams - 2);

    // Initialize children chromosomes
    Chromosome *firstChild = new Chromosome(problem, (int*) firstParent->getGenes());
    Chromosome *secondChild = new Chromosome(problem, (int*) secondParent->getGenes());

    // Extract
    maxCut = maxCutDistribution(generator);
    if(numberOfCuts == 2)
        minCut = minCutDistribution(generator);

    // Initialize flag for ordered crossover
    bool orderedCrossoverPerformed = true;

    if(ordered && firstParent->solution->getCutFeasibility(minCut, maxCut) && secondParent->solution->getCutFeasibility(minCut, maxCut))
        performOrderedCrossover(firstParent, secondParent, firstChild, secondChild, minCut, maxCut, &orderedCrossoverPerformed);
    else
        performStandardCrossover(firstParent, secondParent, firstChild, secondChild, minCut, maxCut);

    if(!orderedCrossoverPerformed)
        performStandardCrossover(firstParent, secondParent, firstChild, secondChild, minCut, maxCut);

    children.push_back(firstChild);
    children.push_back(secondChild);

    return children;

}

Chromosome *Chromosome::inversion(Problem* problem) {

    // Random stuff
    std::uniform_int_distribution<int> cutDistribution(0, solution->exams->size() / 2 - 1);

    //Decide how many cuts will be according with the number of timeslot
    int cut = cutDistribution(generator);
    int numberOfExams = solution->exams->size();

    //Copy old genes and store solution pointer
    Chromosome *invertedChromosome = new Chromosome(problem, (int*) getGenes());

    int *invertedChromosomeGenes = invertedChromosome->solution->examsTimeslots;

    // Swap genes around cutting index
    for(int i = 0; i < cut; i++)
        std::swap(invertedChromosomeGenes[i], invertedChromosomeGenes[numberOfExams - 1 - i]);

    return invertedChromosome;

}

//The fitness is calculated as an integer between 0 and nofGenes
double Chromosome::getFitness() {
    return solution->getGain();
}

Chromosome::~Chromosome() {
    delete solution;
}
