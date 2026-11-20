#ifndef DEPARTMENT_SERVICE_H
#define DEPARTMENT_SERVICE_H

#include "../Repositories/DepartmentRepository.h"
#include "../Repositories/DepartmentIDCounterRepository.h"
#include "../Repositories/EmployeeRepository.h"
#include "../Repositories/PositionRepository.h"

#include <string>
#include <vector>
using namespace std;

class DepartmentService{
private:
    DepartmentRepository departmentRepository;
    DepartmentIDCounterRepository departmentIDCounterRepository;
    EmployeeRepository employeeRepository;
    PositionRepository positionRepository;

    string generateDepartmentID();

public:
    DepartmentService();

    vector<Department> getAllDepartments();

    bool addDepartment(string departmentName, string& message);
    bool updateDepartment(string PHG_ID, string newDepartmentName, string& message);
    bool deleteDepartment(string PHG_ID, string& message);
    bool findByID(string PHG_ID, Department& department);
};

#endif
