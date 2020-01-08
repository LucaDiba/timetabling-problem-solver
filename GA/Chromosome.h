#include <vector>

#include "../data-structures/Exam.h"
#include "../data-structures/Solution.h"

class Chromosome {

public:

    // Constructor
    Chromosome(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents, bool initialize = true, float mutationRate = 0.1f);
    Chromosome(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents, int *initializingSolution, float mutationRate = 0.1f);

    // Destructor
    ~Chromosome();

    // Genetic operators
    int *getGenes();
    void mutation();
    void inversion();
    std::vector<Chromosome> crossover(Chromosome *secondParent, bool ordered = false);
    double getFitness();

private:

    // Solution object
    Solution *solution;

    // Mutation rate
    float mutationRate;

    // Crossover utilities
    void performStandardCrossover(Chromosome *secondParent, Chromosome *firstChild, Chromosome *secondChild, int minCut, int maxCut);
    void performOrderedCrossover(Chromosome *secondParent, Chromosome *firstChild, Chromosome *secondChild, int minCut, int maxCut, bool *performingCrossover);

};
