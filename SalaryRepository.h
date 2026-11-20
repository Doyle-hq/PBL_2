#ifndef SALARY_REPOSITORY_H
#define SALARY_REPOSITORY_H

#include "../Models/Salary.h"
#include <vector>
#include <string>
using namespace std;

class SalaryRepository{
private:
    string fileName;

public:
    SalaryRepository(string fileName = "Data/Salary.txt");

    vector<Salary> getAll();
    bool saveAll(const vector<Salary>& salaries);

    bool add(const Salary& salary);
    bool update(const Salary& salary);
    bool remove(string salaryID);
    bool findByID(string salaryID, Salary& salary);
};

#endif
