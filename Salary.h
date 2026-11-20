#ifndef SALARY_H
#define SALARY_H

#include <string>
using namespace std;

class Salary {
private:
    string salaryID;
    string NV_ID;

    int month;
    int year;

    double salaryCoefficient;
    double baseSalary;

    double allowance;//tro cap
    double bonus;
    double deduction;//khau tru
    double totalSalary;

public:
    Salary();
    Salary(string salaryID, string NV_ID, int month, int year,double salaryCoefficient, double baseSalary, double allowance, double bonus, double deduction, double totalSalary);

    string getSalaryID() const;
    string getNV_ID() const;

    int getMonth() const;
    int getYear() const;

    double getSalaryCoefficient() const;
    double getBaseSalary() const;

    double getAllowance() const;
    double getBonus() const;
    double getDeduction() const;
    double getTotalSalary() const;

    void setSalaryID(string salaryID);
    void setNV_ID(string NV_ID);

    void setMonth(int month);
    void setYear(int year);

    void setSalaryCoefficient(double salaryCoefficient);
    void setBaseSalary(double baseSalary);

    void setAllowance(double allowance);
    void setBonus(double bonus);
    void setDeduction(double deduction);
    void setTotalSalary(double totalSalary);
};

#endif