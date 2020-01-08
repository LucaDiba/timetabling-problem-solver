#ifndef IO_CPP
#define IO_CPP

/*** INPUT ***/

/*  Read the Exams file and return its content as a vector */
std::vector<Exam*> readExams(std::string examsInstance);

/* Get conflicts from enrolled students */
void retrieveConflicts(std::string studentsInstance, std::vector<Exam*> exams, int *students);

/*  Read the Timeslots file and return the number of timeslots  */
int readTimeslots(std::string timeslotsInstance);

/*  Main input function  */
Problem* getProblemFromFile(std::string instance_name, int max_time);

/*** OUTPUT ***/

/*  Check if a given file exists, so we can read it  */
bool isFileExist(std::string fileName);

/*  Read the file with the best solution penalty accomplished  */
double readBestSolutionPenalty(std::string instance_name);

/*  Write the solution to a file, i.e. the timeslots assigned for each exam  */
void writeBestSolution(int *exams_timeslot, int n_exams, std::string instance_name_best_solution);

/*  Write the best solution penalty to a file, so we can compare with future results  */
void writeBestSolutionPenalty(double penalty, std::string instance_name_best_penalty);

/*  Main output function  */
void writeSolutionOnFile(Problem *p);

#endif //IO_CPP
