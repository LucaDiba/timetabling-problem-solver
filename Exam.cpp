#include <iostream>
#include <vector>

#include "Exam.h"

/* Constructor */
Exam::Exam(int examId, int examIndex, int enrolledStudentsIO) {

    id = examId;
    index = examIndex;
    enrolledStudents = enrolledStudentsIO;

}

/* Conflict handling */
void Exam::setConflict(int conflictingExamIndex) {

    if(conflicts.find(conflictingExamIndex) == conflicts.end())
        conflicts[conflictingExamIndex] = 0;

    conflicts[conflictingExamIndex]++;

}

int Exam::getConflict(int examIndex) {
    return conflicts[examIndex];
}

bool Exam::hasConflict(int examIndex) {
    return (conflicts.find(examIndex) != conflicts.end());
}

void Exam::printAllConflicts(std::vector<Exam*> *exams, bool useIndex) {

    /* Choose index or id and print conflicts */
    printf("Conflicts for exam %d: ", useIndex ? index : id);
    for(auto& conflictingExam : conflicts)
            printf("%d ", useIndex ? exams->at(conflictingExam.first)->index : exams->at(conflictingExam.first)->id);

}