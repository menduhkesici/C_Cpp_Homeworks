#ifndef STUDENTRECORD_HPP
#define STUDENTRECORD_HPP

#include <string.h>

using namespace std;

class StudentRecord
{
public:
    bool valid;
    int studentID;
    char studentName[100];
    int studentGrades[3]; //array element i stores the grade of course id i (course ids are selected from 0 to 2).
    StudentRecord (void); // void constructor initializes with default values.
    StudentRecord (bool v, int ID, char* name, int grades[3]);
};

StudentRecord::StudentRecord(void)
{
    valid = false;
    studentID = -1;
    strcpy(studentName, " ");
    for (int i = 0; i < 3; i++)
    {
        studentGrades[i] = 0;
    }
}

StudentRecord::StudentRecord(bool v, int ID, char* name, int grades[3])
{
    valid = v;
    studentID = ID;
    strcpy(studentName, name);
    for (int i = 0; i < 3; i++)
    {
        studentGrades[i] = grades[i];
    }
}

#endif // STUDENTRECORD_HPP
