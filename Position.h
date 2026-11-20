#ifndef POSITION_H
#define POSITION_H

#include <string>
using namespace std;

class Position{
private:
    string PHG_ID;
    string positionName;
    string employeeType;
    double salaryCoefficient;//he so luong

public:
    Position();
    Position(string PHG_ID, string positionName, string employeeType, double salaryCoefficient);

    string getPHG_ID() const;
    string getPositionName() const;
    string getEmployeeType() const;
    double getSalaryCoefficient() const;

    void setPHG_ID(string PHG_ID);
    void setPositionName(string positionName);
    void setEmployeeType(string employeeType);
    void setSalaryCoefficient(double salaryCoefficient);
};

#endif