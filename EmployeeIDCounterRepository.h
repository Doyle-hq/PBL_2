#ifndef EMPLOYEE_ID_COUNTER_REPOSITORY_H
#define EMPLOYEE_ID_COUNTER_REPOSITORY_H

#include <string>
#include <vector>
#include <utility>
using namespace std;

class EmployeeIDCounterRepository{
private:
    string fileName;

public:
    EmployeeIDCounterRepository(string fileName = "Data/EmployeeIDCounter.txt");

    int getNextNumber(string PHG_ID, int currentMax);
};

#endif