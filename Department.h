#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>
using namespace std;

class Department{
private:
    string PHG_ID;
    string departmentName;//ten phong ban

public:
    Department();
    Department(string PHG_ID, string departmentName);

    string getPHG_ID() const;
    string getDepartmentName() const;

    void setPHG_ID(string PHG_ID);
    void setDepartmentName(string departmentName);
};

#endif