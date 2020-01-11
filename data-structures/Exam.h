#ifndef EXAM_CPP
#define EXAM_CPP

#include <unordered_map>

class Exam {

public:
    // id as declared on the input
    int id;

    // index of current exam on the exams vector
    int index;

    // number of student enrolled to this exam
    int enrolledStudents;

    std::unordered_map<int, int> conflicts;

    /* Constructor */
    Exam(int examId, int examIndex, int enrolledStudentsIO);

    /* Conflict handling */
    void setConflict(int conflictingExamId);
    int getConflict(int examId);
    bool hasConflict(int examId);
    void printAllConflicts(std::vector<Exam*> *exams, bool useIndex = false);

};


#endif //EXAM_CPP
