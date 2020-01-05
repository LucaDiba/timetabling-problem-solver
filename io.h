#ifndef IO_CPP
#define IO_CPP

#include <ctime>
#include "data_structures.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>

/* INPUT */

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

    return p;
}

/* OUTPUT */
void writeSolutionOnFile(Solution sol) {

}

#endif //IO_CPP