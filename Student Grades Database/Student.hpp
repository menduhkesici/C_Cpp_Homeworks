#ifndef STUDENT_HPP
#define STUDENT_HPP

class Student
{
public:
    int studentID;
    bool record_valid;
    int index;
    Student (int ID = -1, bool valid = false, int i = -1); //constructor
};

Student::Student(int ID, bool valid, int i)
{
    studentID = ID;
    record_valid = valid;
    index = i;
}

#endif // STUDENT_HPP
