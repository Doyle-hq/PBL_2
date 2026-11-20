#include "SalaryPolicy.h"

SalaryPolicy::~SalaryPolicy(){
}

double WorkerSalaryPolicy::calculateSalary(double baseSalary, double allowance, double bonus, double deduction) const{
    return baseSalary + allowance + bonus - deduction;
}

double OfficeSalaryPolicy::calculateSalary(double baseSalary, double allowance, double bonus, double deduction) const{
    return baseSalary + allowance + bonus - deduction;
}