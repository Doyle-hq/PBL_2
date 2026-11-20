#include "Position.h"

Position::Position(){
    positionName = "";
    employeeType = "";
    salaryCoefficient = 0;
}

Position::Position(string PHG_ID, string positionName, string employeeType, double salaryCoefficient){
    this->PHG_ID = PHG_ID;
    this->positionName = positionName;
    this->employeeType = employeeType;
    this->salaryCoefficient = salaryCoefficient;
}

string Position::getPHG_ID() const {
    return PHG_ID;
}

string Position::getPositionName() const{
    return positionName;
}

string Position::getEmployeeType() const{
    return employeeType;
}

double Position::getSalaryCoefficient() const{
    return salaryCoefficient;
}

void Position::setPHG_ID(string PHG_ID) {
    this->PHG_ID = PHG_ID;
}

void Position::setPositionName(string positionName) {
    this->positionName = positionName;
}

void Position::setEmployeeType(string employeeType) {
    this->employeeType = employeeType;
}

void Position::setSalaryCoefficient(double salaryCoefficient) {
    this->salaryCoefficient = salaryCoefficient;
}