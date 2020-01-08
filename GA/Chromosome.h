#include <vector>

#include "../data-structures/Exam.h"
#include "../data-structures/Solution.h"
#include "../data-structures/Problem.h"

class Chromosome {

public:

    // Constructor
    explicit Chromosome(Problem* problem, bool initialize = true, float mutationRate = 0.1f);
    Chromosome(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents, bool initialize = true, float mutationRate = 0.1f);
    Chromosome(std::vector<Exam*> *examsVector, int numberOfTimeslots, int numberOfStudents, int *initializingSolution, float mutationRate = 0.1f);

    // Destructor
    ~Chromosome();

    // Genetic operators

    /**
     * Generates a copy of current genes
     * @return array of dimension "n_exams", each containing the assigned timeslot
     */
    int *getGenes();

    /**
     * Mutate one random gene, assigning to it one random timeslot, in order to diversify the search and avoid getting stuch in a local minimum
     * WARNING: FEASIBILITY NOT GUARANTEED!
     */
    void mutation();

    /**
     * TODO: documentation - what does this function do?
     */
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
    void performOrderedCrossover(Chromosome *secondParent, Chromosome *firstChild, Chromosome *secondChild, int minCut, int maxCut);

};
