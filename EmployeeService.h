#ifndef EMPLOYEE_SERVICE_H
#define EMPLOYEE_SERVICE_H

#include "../Repositories/EmployeeRepository.h"
#include "../Repositories/AccountRepository.h"
#include "../Repositories/PositionRepository.h"
#include "../Repositories/EmployeeIDCounterRepository.h"
#include <vector>
#include <string>
using namespace std;

class EmployeeService {
private:
    EmployeeRepository employeeRepository;
    AccountRepository accountRepository;
    PositionRepository positionRepository;
    EmployeeIDCounterRepository employeeIDCounterRepository;

    bool validateEmployee(const Employee& employee, string& message);
    string generateEmployeeID(string PHG_ID);
    string getCurrentDate();

public:
    EmployeeService();

    vector<Employee> getAllEmployees() const;

    // SAP XEP NHAN VIEN
    vector<Employee> sortEmployeesByID() const;
    vector<Employee> sortEmployeesByName() const;
    vector<Employee> sortEmployeesByDepartment() const;

    bool prepareEmployee(Employee& employee, string& message);
    bool addEmployee(Employee& employee, string& message);
    bool updateEmployee(const Employee& employee, string& message);
    bool updateMyContactInfo(const string& NV_ID, const string& phone, const string& address, string& message);
    bool deleteEmployee(string NV_ID, string& message);

    bool findByID(string NV_ID, Employee& employee);
    vector<Employee> searchByName(string name);

    vector<Employee> sortByID() const;
    vector<Employee> sortByName() const;
    vector<Employee> sortByDepartment() const;

    vector<Employee> filterByDepartment(string PHG_ID) const;

    bool confirmResignation(string NV_ID, string& message);
};

#endif