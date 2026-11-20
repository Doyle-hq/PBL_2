#ifndef DEPARTMENT_REPOSITORY_H
#define DEPARTMENT_REPOSITORY_H

#include "../Models/Department.h"
#include <vector>
#include <string>
using namespace std;

class DepartmentRepository{
private:
    string fileName;

public:
    DepartmentRepository(string fileName = "Data/Department.txt");

    vector<Department> getAll();
    bool saveAll(const vector<Department>& departments);

    bool add(const Department& department);
    bool update(const Department& department);
    bool remove(string PHG_ID);
    bool findByID(string PHG_ID, Department& department);
};

#endif
