#ifndef EXAM_CPP
#define EXAM_CPP

#include <unordered_map>

class Exam {

public:

    int id, index;
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
