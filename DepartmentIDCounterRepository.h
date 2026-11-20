#ifndef DEPARTMENT_ID_COUNTER_REPOSITORY_H
#define DEPARTMENT_ID_COUNTER_REPOSITORY_H

#include <string>
using namespace std;

class DepartmentIDCounterRepository{
private:
    string fileName;

public:
    DepartmentIDCounterRepository(string fileName = "Data/DepartmentIDCounter.txt");
    int getNextNumber(int currentMax);
};

#endif
