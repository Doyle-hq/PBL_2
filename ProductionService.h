#ifndef PRODUCTION_SERVICE_H
#define PRODUCTION_SERVICE_H

#include "../Repositories/ProductionRepository.h"
#include "../Repositories/ProductRepository.h"
#include "../Repositories/EmployeeRepository.h"
#include "../Repositories/PositionRepository.h"
#include "../Repositories/SalaryRepository.h"
#include "../Models/Salary.h"
#include <vector>
#include <string>
using namespace std;

class ProductionService{
private:
    ProductionRepository productionRepository;
    ProductRepository productRepository;
    EmployeeRepository employeeRepository;
    PositionRepository positionRepository;
    SalaryRepository salaryRepository;

    string generateProductionID();

public:
    ProductionService();

    vector<ProductionRecord> getAllRecords();

    vector<ProductionRecord> getEmployeeProduction(string NV_ID, int month, int year);

    bool addProduction(string NV_ID, string productID, int quantity, int month, int year, string& message);
    bool updateProduction(string productionID, int quantity, string& message);
    bool deleteProduction(string productionID, string& message);
    double calculateWorkerProductionAmount(string NV_ID, int month, int year);
    bool findProductionByID(string productionID, ProductionRecord& record);
    bool isSalaryCreated(string NV_ID, int month, int year);
};

#endif
