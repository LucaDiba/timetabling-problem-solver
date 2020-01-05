#ifndef IO_CPP
#define IO_CPP

#include "data_structures.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


/*** INPUT ***/

/*  Dynamically allocate memory for a matrix  */
int** createMatrix(int lines, int columns) {
    int i;
    int **matrix;

    matrix = (int**) calloc(lines, sizeof(int*));

    for (i = 0; i < lines; i++)
        matrix[i] = (int*) calloc(columns, sizeof(int));

    return matrix;
}

/*  Count the number of lines in a file  */
int countLines(std::string instance_name){
    bool emptyLine = false;
    int count = 0;
    std::ifstream myfile;
    std::string line;

    myfile.open(instance_name, std::ios_base::in);

    if (!myfile) {
        std::cout << "Unable to open file";
    }
    else {
        while (getline(myfile, line)){
            count++;

            /*  Verify if the last line of the file is empty  */
            emptyLine = line.empty();
        }
    }

    /*  If the last line of the file is empty, decrease the counter  */ 
    if(emptyLine)
        count--;

    return count;
}

/*  Read the Exams file and return its content as a matrix */
int **readExams(std::string instance_name, int n_lines_exams){
    int **exams;
    int i;
    std::ifstream myfile;

    exams = createMatrix(n_lines_exams, 2);

    myfile.open(instance_name, std::ios_base::in);
    if (!myfile) {
        std::cout << "Unable to open file ";
    }
    else {
        for (i = 0; i < n_lines_exams; i++){
            myfile >> exams[i][0] >> exams[i][1];
        }
    }
    
    return exams;
}

/*  Read the Students file and return its content as a matrix */
int** readStudents(std::string instance_name, int n_lines_students){
    int **students;
    int i, student_exam;
    std::string student_id;
    std::ifstream myfile;

    students = createMatrix(n_lines_students, 2);

    myfile.open(instance_name, std::ios_base::in);

    if (!myfile) {
        std::cout << "Unable to open file";
    }
    else {
        for (i = 0; i < n_lines_students; i++) {
            myfile >> student_id >> student_exam;

            /*  Since the student id is a string like 's{id}' we must remove the char 's' before storing the value in the matrix  */
            students[i][0] = std::stoi(student_id.erase(0, 1));
            students[i][1] = student_exam;
        }
    }

    return students;
}

/*  Read the Timeslots file and return the number of timeslots  */
int readTimeSlots(std::string instance_name){
    int timeslots;
    std::ifstream myfile;

    myfile.open(instance_name, std::ios_base::in);

    if (!myfile)
        std::cout << "Unable to open file";

    myfile >> timeslots;
    myfile.close();

    return timeslots;
}

/*  Get the number of unique student ids in the Student file  */
int getNumberOfStudents(int **students, int n_lines_students){
    std::vector<int> studentIds;
    int i;

    /*  Create a new vector of student ids using the std::vector structure, that is easier to handle  */
    for(i = 0; i < n_lines_students; i++)
        studentIds.push_back(students[i][0]);
    
    /*  Sort the new vector and then use the std::unique function to get the unique values  */
    std::sort(studentIds.begin(), studentIds.end());
    int uniqueCount = std::unique(studentIds.begin(), studentIds.end()) - studentIds.begin();

    return uniqueCount;
}

/*  Create the matrix ixj with students and exams, which returns 1 if student i is enrolled in exam j  */
int** getStudentEnrolledMatrix(int **students, int n_exams, int n_students, int n_lines_students){
    int **student_enrolled = createMatrix(n_students, n_exams);
    int i, studentId, exam;

    /*  For each line (student i enrolled in exam j) in the Students file, sets 1 to the correspondent cell in the matrix  */
    for (i = 0; i < n_lines_students; i++){
        studentId = students[i][0];
        exam = students[i][1];

        student_enrolled[studentId - 1][exam - 1] = 1;
    }

    return student_enrolled;
}

/*  Create the matrix with exam conflicts, i.e. the number of students enrolled in both exams i and j  */
int** getExamConflicts(int **student_enrolled, int n_exams, int n_students){
    int **conflicts = createMatrix(n_exams, n_exams);
    int i, j, k;

    /*  For each student, add 1 if the student is enrolled in both exams j and k  */
    for (i = 0; i < n_students; i++){
        for(j = 0; j < n_exams - 1; j++){

            /*  Check if the student i is enrolled in exam j, otherwise we can skip the next loop  */
            if(student_enrolled[i][j] == 1){
                for(k = j + 1; k < n_exams; k++){

                    /*  Increase 1 to the conflicts matrix if the student i is enrolled in both exams j and k  */
                    conflicts[j][k] += student_enrolled[i][j] * student_enrolled[i][k];
                }
            }
        }
    }

    return conflicts;
}

/*  Main input function  */
Problem* getProblemFromFile(std::string instance_name, int max_time) {
    Problem* p = new Problem;

    p->instance_name = std::move(instance_name);
    p->max_time = max_time;
    p->start_time = time(NULL);

    /*  Get instance filenames  */
    std::string instance_name_exams = p->instance_name + ".exm";
    std::string instance_name_students = p->instance_name + ".stu";
    std::string instance_name_timeslots = p->instance_name + ".slo";

    int n_lines_exams = countLines(instance_name_exams);
    int n_lines_students = countLines(instance_name_students);

    /*  Read the files  */
    int **exams = readExams(instance_name_exams, n_lines_exams);
    int **students = readStudents(instance_name_students, n_lines_students);
    int timeslots = readTimeSlots(instance_name_timeslots);

    /*  Get the Problem values  */
    p->n_exams = n_lines_exams;
    p->n_students = getNumberOfStudents(students, n_lines_students);
    p->n_timeslots = timeslots;
    p->student_enrolled = getStudentEnrolledMatrix(students, p->n_exams, p->n_students, n_lines_students);
    p->conflicts = getExamConflicts(p->student_enrolled, p->n_exams, p->n_students);

    //MOCK
    p->best_solution = new Solution();
    p->best_solution->penalty = 100.125;
    p->best_solution->exams_timeslot = (int*) calloc(139, sizeof(int));
    for(int i = 0; i < 139; i++){
        p->best_solution->exams_timeslot[i] = (i % 5) + 1;
    }

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
    std::ifstream myfile;

    myfile.open(instance_name, std::ios_base::in);

    if (!myfile)
        std::cout << "Unable to open file";

    myfile >> bestSolutionPenalty;
    myfile.close();

    return bestSolutionPenalty;
}

/*  Write the solution to a file, i.e. the timeslots assigned for each exam  */
void writeBestSolution(int *exams_timeslot, int n_exams, std::string instance_name_best_solution){
    std::ofstream myfile;

    myfile.open (instance_name_best_solution);
    
    for(int i = 0; i < n_exams; i++){

        /*  Since it is asked to write the exams id exactly as they are given, we complement the ids with leading zeros until we have 4 characters  */
        myfile << std::setfill('0') << std::setw(4) << i + 1 << " " << exams_timeslot[i] << std::endl;
    }

    myfile.close();
}

/*  Write the best solution penalty to a file, so we can compare with future results  */
void writeBestSolutionPenalty(double penalty, std::string instance_name_best_penalty){
    std::ofstream myfile;

    myfile.open (instance_name_best_penalty);

    myfile << penalty;

    myfile.close();
}

/*  Main output function  */
void writeSolutionOnFile(Problem *p) {
    std::string instance_name_best_penalty = p->instance_name + "_DMOgroup03.bst";
    std::string instance_name_best_solution = p->instance_name + "_DMOgroup03.sol";

    double bestSolutionPenalty = -1;

    /*  If another solution was obtained before, read its penalty so we can compare with the actual solution  */
    if(isFileExist(instance_name_best_penalty))
        bestSolutionPenalty = readBestSolutionPenalty(instance_name_best_penalty);
    
    /*  Write/Overwrite the solution if there was not one before or the current solution is better  */
    if(bestSolutionPenalty == -1 || p->best_solution->penalty < bestSolutionPenalty){
        writeBestSolution(p->best_solution->exams_timeslot, p->n_exams, instance_name_best_solution);
        writeBestSolutionPenalty(p->best_solution->penalty, instance_name_best_penalty);
    }
}

#endif //IO_CPP