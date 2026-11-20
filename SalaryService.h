#ifndef SALARY_SERVICE_H
#define SALARY_SERVICE_H

#include "../Repositories/SalaryRepository.h"
#include "../Repositories/EmployeeRepository.h"
#include "../Repositories/PositionRepository.h"
#include "../Services/ProductionService.h"
#include "../Models/Salary.h"
#include "../Models/SalaryPolicy.h"

#include <vector>
#include <string>
using namespace std;

class SalaryService{
private:
    SalaryRepository salaryRepository;
    EmployeeRepository employeeRepository;
    PositionRepository positionRepository;
    ProductionService productionService;

    string generateSalaryID();

public:
    SalaryService();

    vector<Salary> getAllSalaries();

    vector<Salary> getSalariesByPeriod(int month, int year);

    vector<Salary> getEmployeeSalaryHistory(string NV_ID);

    bool findSalaryByID(string salaryID, Salary& salary);
    bool createSalary(string NV_ID, int month, int year, double baseSalary, double allowance, double bonus, double deduction, string& message);
    bool isSalaryCreated(string NV_ID, int month, int year);
    double calculateTotalSalary(string employeeType, double baseSalary, double salaryCoefficient, double allowance, double bonus, double deduction);
};

#endif
