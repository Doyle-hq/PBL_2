#include "Salary.h"

// Constructor mac dinh
Salary::Salary(){
    salaryID = "";
    NV_ID = "";
    month = 0;
    year = 0;
    salaryCoefficient = 0.0;
    baseSalary = 0.0;
    allowance = 0.0;
    bonus = 0.0;
    deduction = 0.0;
    totalSalary = 0.0;
}

// Constructor co tham so
Salary::Salary(string salaryID, string NV_ID, int month, int year, double salaryCoefficient, double baseSalary, double allowance, double bonus, double deduction, double totalSalary){
    this->salaryID = salaryID;
    this->NV_ID = NV_ID;
    this->month = month;
    this->year = year;
    this->salaryCoefficient = salaryCoefficient;
    this->baseSalary = baseSalary;
    this->allowance = allowance;
    this->bonus = bonus;
    this->deduction = deduction;
    this->totalSalary = totalSalary;
}

// Getter
string Salary::getSalaryID() const{
    return salaryID;
}

string Salary::getNV_ID() const{
    return NV_ID;
}

int Salary::getMonth() const{
    return month;
}

int Salary::getYear() const{
    return year;
}

double Salary::getSalaryCoefficient() const{
    return salaryCoefficient;
}

double Salary::getBaseSalary() const{
    return baseSalary;
}

double Salary::getAllowance() const{
    return allowance;
}

double Salary::getBonus() const{
    return bonus;
}

double Salary::getDeduction() const{
    return deduction;
}

double Salary::getTotalSalary() const{
    return totalSalary;
}

// Setter
void Salary::setSalaryID(string salaryID){
    this->salaryID = salaryID;
}

void Salary::setNV_ID(string NV_ID){
    this->NV_ID = NV_ID;
}

void Salary::setMonth(int month){
    this->month = month;
}

void Salary::setYear(int year){
    this->year = year;
}

void Salary::setSalaryCoefficient(double salaryCoefficient){
    this->salaryCoefficient = salaryCoefficient;
}

void Salary::setBaseSalary(double baseSalary){
    this->baseSalary = baseSalary;
}

void Salary::setAllowance(double allowance){
    this->allowance = allowance;
}

void Salary::setBonus(double bonus){
    this->bonus = bonus;
}

void Salary::setDeduction(double deduction){
    this->deduction = deduction;
}

void Salary::setTotalSalary(double totalSalary){
    this->totalSalary = totalSalary;
}