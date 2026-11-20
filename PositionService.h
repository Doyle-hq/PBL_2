#ifndef POSITION_SERVICE_H
#define POSITION_SERVICE_H

#include "../Repositories/PositionRepository.h"
#include "../Repositories/DepartmentRepository.h"
#include "../Repositories/EmployeeRepository.h"
#include <vector>
#include <string>
using namespace std;

class PositionService{
private:
    PositionRepository positionRepository; //luu tru vi tri
    DepartmentRepository departmentRepository;// luu tru bo phan
    EmployeeRepository employeeRepository;// luu tru nhan vien

public:
    PositionService();

    vector<Position> getAllPositions();
    vector<Position> getPositionsByDepartment(string PHG_ID);

    bool addPosition(const Position& position);
    bool updatePosition(string PHG_ID, string oldPositionName, const Position& newPosition);
    bool deletePosition(string PHG_ID, string positionName);
    bool findPosition(string PHG_ID, string positionName, Position& position);
};

#endif
