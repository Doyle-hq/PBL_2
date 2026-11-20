#include "SalaryRepository.h"
#include <fstream>
#include <sstream>
#include <iomanip>

SalaryRepository::SalaryRepository(string fileName){
    this->fileName = fileName;
}

vector<Salary> SalaryRepository::getAll(){
    vector<Salary> salaries;
    ifstream file(fileName);

    if(!file.is_open()){
        return salaries;
    }

    string line;

    while(getline(file, line)){
        if(line.empty()) continue;

        stringstream ss(line);

        string salaryID, NV_ID;
        string monthText, yearText;
        string coefficientText, baseSalaryText;
        string allowanceText, bonusText;
        string deductionText, totalSalaryText;

        getline(ss, salaryID, ',');
        getline(ss, NV_ID, ',');
        getline(ss, monthText, ',');
        getline(ss, yearText, ',');
        getline(ss, coefficientText, ',');
        getline(ss, baseSalaryText, ',');
        getline(ss, allowanceText, ',');
        getline(ss, bonusText, ',');
        getline(ss, deductionText, ',');
        getline(ss, totalSalaryText, ',');

        if(salaryID.empty() || salaryID == "Salary"){
            continue;
        }

        try{
            int month = stoi(monthText);
            int year = stoi(yearText);

            double salaryCoefficient = stod(coefficientText);
            double baseSalary = stod(baseSalaryText);
            double allowance = stod(allowanceText);
            double bonus = stod(bonusText);
            double deduction = stod(deductionText);
            double totalSalary = stod(totalSalaryText);

            Salary salary(salaryID, NV_ID, month, year, salaryCoefficient, baseSalary, allowance, bonus, deduction, totalSalary);

            salaries.push_back(salary);
        }
        catch(...){
            continue;
        }
    }

    file.close();
    return salaries;
}

bool SalaryRepository::saveAll(const vector<Salary>& salaries){
    ofstream file(fileName);

    if(!file.is_open()){
        return false;
    }

    file << "Salary" << endl;
    file << fixed << setprecision(2);

    for(const Salary& salary : salaries){
        file << salary.getSalaryID() << "," << salary.getNV_ID() << "," << salary.getMonth() << "," << salary.getYear() << "," << salary.getSalaryCoefficient() << "," << salary.getBaseSalary() << "," << salary.getAllowance() << "," << salary.getBonus() << "," << salary.getDeduction() << "," << salary.getTotalSalary() << endl;
    }

    file.close();
    return true;
}

bool SalaryRepository::add(const Salary& salary){
    vector<Salary> salaries = getAll();

    for(const Salary& current : salaries){
        // Kiem tra trung ma bang luong
        if(current.getSalaryID() == salary.getSalaryID()){
            return false;
        }

        // Moi nhan vien chi co mot bang luong trong mot ky
        if(current.getNV_ID() == salary.getNV_ID() && current.getMonth() == salary.getMonth() && current.getYear() == salary.getYear()){
            return false;
        }
    }

    salaries.push_back(salary);
    return saveAll(salaries);
}

bool SalaryRepository::update(const Salary& salary){
    vector<Salary> salaries = getAll();

    int index = -1;

    // Tim bang luong can cap nhat
    for(int i = 0; i < (int)salaries.size(); i++){
        if(salaries[i].getSalaryID() == salary.getSalaryID()){
            index = i;
            break;
        }
    }

    if(index == -1){
        return false;
    }

    // Kiem tra trung nhan vien trong cung ky
    for(int i = 0; i < (int)salaries.size(); i++){
        if(i == index) continue;

        if(salaries[i].getNV_ID() == salary.getNV_ID() && salaries[i].getMonth() == salary.getMonth() && salaries[i].getYear() == salary.getYear()){
            return false;
        }
    }

    salaries[index] = salary;

    return saveAll(salaries);
}

bool SalaryRepository::remove(string salaryID){
    vector<Salary> salaries = getAll();

    for(int i = 0; i < (int)salaries.size(); i++){
        if(salaries[i].getSalaryID() == salaryID){
            salaries.erase(salaries.begin() + i);
            return saveAll(salaries);
        }
    }

    return false;
}

bool SalaryRepository::findByID(string salaryID, Salary& salary){
    vector<Salary> salaries = getAll();

    for(const Salary& current : salaries){
        if(current.getSalaryID() == salaryID){
            salary = current;
            return true;
        }
    }

    return false;
}