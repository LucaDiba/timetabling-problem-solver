#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "data-structures/Exam.h"
#include "data-structures/Problem.h"

/*  Read the Exams file and return its content as a vector */
std::vector<Exam*> readExams(std::string examsInstance){

    // Setting up exams dynamic vector
    std::vector<Exam*> exams;

    // File IO
    std::ifstream file;
    file.open(examsInstance, std::ios_base::in);

    int examId, examEnrolledStudents;

    if (!file)
        std::cout << "Unable to open file ";
    else {
        while(file >> examId >> examEnrolledStudents)
            exams.push_back(new Exam(examId, examId - 1, examEnrolledStudents));
    }

    // Close file
    file.close();

    return exams;

}

/* Get conflicts from enrolled students */
void retrieveConflicts(std::string studentsInstance, std::vector<Exam*> exams, int *students) {

    // File IO
    std::ifstream file;
    file.open(studentsInstance, std::ios_base::in);

    // Iteration variables
    std::string prevStudentId;
    std::string currentStudentId;
    int examId;

    if (!file)
        std::cout << "Unable to open file";
    else {

        // Create a collection to store conflicting exams for each student
        std::vector<int> conflictingExams;

        while(file >> currentStudentId >> examId) {

            // Set conflicts on equal student id, otherwise clear conflicts vector and update student ID
            if(currentStudentId == prevStudentId){
                for(auto& conflictingExam: conflictingExams){
                    exams[conflictingExam - 1]->setConflict(examId - 1);
                    exams[examId - 1]->setConflict(conflictingExam - 1);
                }
            } else {
                (*students)++;
                conflictingExams.clear();
            }

            conflictingExams.push_back(examId);
            prevStudentId = currentStudentId;

        }

    }

    // Close file
    file.close();

}

/*  Read the Timeslots file and return the number of timeslots  */
int readTimeslots(std::string timeslotsInstance){

    int timeslots;

    // File IO
    std::ifstream file;
    file.open(timeslotsInstance, std::ios_base::in);

    if (!file)
        std::cout << "Unable to open file";

    file >> timeslots;

    // Close file
    file.close();

    return timeslots;

}

/*  Main input function  */
Problem* getProblemFromFile(std::string instance_name, int max_time) {

    Problem* p = new Problem;

    p->instanceName = std::move(instance_name);
    p->max_time = max_time;
    p->start_time = time(NULL);

    // Read the files
    p->exams = readExams(p->instanceName + ".exm");
    p->timeslots = readTimeslots(p->instanceName + ".slo");
    retrieveConflicts(p->instanceName + ".stu", p->exams, &p->students);

    // Declare solutions
    p->bestSolution = new Solution(&(p->exams), p->timeslots, p->students);
    p->currentSolution = new Solution(&(p->exams), p->timeslots, p->students);

    return p;

}

/*** OUTPUT ***/

/*  Check if a given file exists, so we can read it  */
bool isFileExist(std::string fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

/*  Read the file with the best solution penalty accomplished  */
double readBestSolutionPenalty(std::string instance_name){
    double bestSolutionPenalty;
    std::ifstream file;

    file.open(instance_name, std::ios_base::in);

    if (!file)
        std::cout << "Unable to open file";

    file >> bestSolutionPenalty;
    file.close();

    return bestSolutionPenalty;
}

/*  Write the solution to a file, i.e. the timeslots assigned for each exam  */
void writeBestSolution(int *exams_timeslot, int n_exams, std::string instance_name_best_solution){
    std::ofstream file;

    file.open (instance_name_best_solution);

    for(int i = 0; i < n_exams; i++){

        /*  Since it is asked to write the exams id exactly as they are given, we complement the ids with leading zeros until we have 4 characters  */
        file << std::setfill('0') << std::setw(4) << i + 1 << " " << exams_timeslot[i] << std::endl;
    }

    file.close();
}

/*  Write the best solution penalty to a file, so we can compare with future results  */
void writeBestSolutionPenalty(double penalty, std::string instance_name_best_penalty){

    // File IO
    std::ofstream file;
    file.open (instance_name_best_penalty);

    file << penalty;
    file.close();

}

/*  Main output function  */
void writeSolutionOnFile(Problem *p) {
    std::string instance_name_best_penalty = p->instanceName + "_DMOgroup03.bst";
    std::string instance_name_best_solution = p->instanceName + "_DMOgroup03.sol";

    double bestSolutionPenalty = -1;

    /*  If another solution was obtained before, read its penalty so we can compare with the actual solution  */
    if(isFileExist(instance_name_best_penalty))
        bestSolutionPenalty = readBestSolutionPenalty(instance_name_best_penalty);

    /*  Write/Overwrite the solution if there was not one before or the current solution is better  */
    if(bestSolutionPenalty == -1 || p->bestSolution->penalty < bestSolutionPenalty){
        writeBestSolution(p->bestSolution->examsTimeslots, p->exams.size(), instance_name_best_solution);
        writeBestSolutionPenalty(p->bestSolution->penalty, instance_name_best_penalty);
    }
}