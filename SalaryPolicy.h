#ifndef SALARY_POLICY_H
#define SALARY_POLICY_H

class SalaryPolicy{
public:
    virtual double calculateSalary(double baseSalary, double allowance, double bonus, double deduction) const = 0;

    virtual ~SalaryPolicy();
};

class WorkerSalaryPolicy : public SalaryPolicy{
public:
    double calculateSalary(double baseSalary, double allowance, double bonus, double deduction) const override;
};

class OfficeSalaryPolicy : public SalaryPolicy{
public:
    double calculateSalary(double baseSalary, double allowance, double bonus, double deduction) const override;
};

#endif