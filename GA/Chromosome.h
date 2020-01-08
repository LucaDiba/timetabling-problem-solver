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

    // Solution object
    Solution *solution;

    // Mutation rate
    float mutationRate;

    // Genetic operators
    int *getGenes();
    void mutation();
    static std::vector<Chromosome> crossover(Chromosome *firstParent, Chromosome *secondParent, bool ordered = false);
    void inversion();
    double getFitness();

};
