#include "SalaryService.h"

#include <sstream>
#include <iomanip>

SalaryService::SalaryService(){
    salaryRepository = SalaryRepository("Data/Salary.txt");
    employeeRepository = EmployeeRepository("Data/Employee.txt");
    positionRepository = PositionRepository("Data/Position.txt");
}

// Tao ma luong tu dong
string SalaryService::generateSalaryID(){
    vector<Salary> salaries = salaryRepository.getAll();

    int maxNumber = 0;

    for(const Salary& salary : salaries){
        string id = salary.getSalaryID();

        if(id.length() > 2 && id.substr(0, 2) == "SL"){
            try{
                int number = stoi(id.substr(2));

                if(number > maxNumber){
                    maxNumber = number;
                }
            }
            catch(...){
                continue;
            }
        }
    }

    stringstream ss;
    ss << "SL" << setw(6) << setfill('0') << maxNumber + 1;

    return ss.str();
}

// Lay tat ca bang luong
vector<Salary> SalaryService::getAllSalaries(){
    return salaryRepository.getAll();
}

// Lay bang luong theo thang va nam
vector<Salary> SalaryService::getSalariesByPeriod(int month, int year){
    vector<Salary> salaries = salaryRepository.getAll();
    vector<Salary> result;

    for(const Salary& salary : salaries){
        if(salary.getMonth() == month && salary.getYear() == year){
            result.push_back(salary);
        }
    }

    return result;
}

// Lay lich su luong cua mot nhan vien
vector<Salary> SalaryService::getEmployeeSalaryHistory(string NV_ID){
    vector<Salary> salaries = salaryRepository.getAll();
    vector<Salary> result;

    for(const Salary& salary : salaries){
        if(salary.getNV_ID() == NV_ID){
            result.push_back(salary);
        }
    }

    return result;
}

// Tim bang luong theo ma
bool SalaryService::findSalaryByID(string salaryID, Salary& salary){
    return salaryRepository.findByID(salaryID, salary);
}

bool SalaryService::isSalaryCreated(string NV_ID, int month, int year){
    vector<Salary> salaries = salaryRepository.getAll();

    for(const Salary& salary : salaries){
        if(salary.getNV_ID() == NV_ID && salary.getMonth() == month && salary.getYear() == year){
            return true;
        }
    }
    return false;
}

// Tinh luong theo chinh sach
double SalaryService::calculateTotalSalary(string employeeType, double baseSalary, double salaryCoefficient, double allowance, double bonus, double deduction){
    WorkerSalaryPolicy workerPolicy;
    OfficeSalaryPolicy officePolicy;

    SalaryPolicy* policy = nullptr;

    double amountBeforeBenefits = baseSalary;

    if(employeeType == "Cong nhan"){
        policy = &workerPolicy;
    }
    else if(employeeType == "Van phong"){
        policy = &officePolicy;

        // Luong nhan vien van phong = luong co ban x he so
        amountBeforeBenefits = baseSalary * salaryCoefficient;
    }
    else{
        return -1;
    }

    return policy->calculateSalary(amountBeforeBenefits, allowance, bonus, deduction);
}

// Lap bang luong cho nhan vien
bool SalaryService::createSalary(string NV_ID, int month, int year, double baseSalary, double allowance, double bonus, double deduction, string& message){
    // 1. Kiem tra du lieu dau vao
    if(NV_ID.empty()){
        message = "Ma nhan vien khong hop le!";
        return false;
    }

    if(month < 1 || month > 12){
        message = "Thang khong hop le!";
        return false;
    }

    if(year < 2000){
        message = "Nam khong hop le!";
        return false;
    }

    if(baseSalary < 0 || allowance < 0 || bonus < 0 || deduction < 0){
        message = "Cac khoan tien khong duoc am!";
        return false;
    }

    // 2. Kiem tra nhan vien
    Employee employee;

    if(!employeeRepository.findByID(NV_ID, employee)){
        message = "Khong tim thay nhan vien!";
        return false;
    }

    // 3. Kiem tra nhan vien da co bang luong ky nay chua
    if(isSalaryCreated(NV_ID, month, year)){
    message = "Nhan vien da co bang luong trong ky nay!";
    return false;
    }

    // 4. Tim chuc vu va loai nhan vien
    vector<Position> positions = positionRepository.getAll();

    string employeeType = "";
    double salaryCoefficient = 0;
    bool foundPosition = false;

    for(const Position& position : positions){
        if(position.getPHG_ID() == employee.getPHG_ID() && position.getPositionName() == employee.getPosition()){
            employeeType = position.getEmployeeType();
            salaryCoefficient = position.getSalaryCoefficient();
            foundPosition = true;
            break;
        }
    }

    if(!foundPosition){
        message = "Khong tim thay chuc vu cua nhan vien!";
        return false;
    }

    // 5. Xac dinh tien luong theo loai nhan vien
    if(employeeType == "Cong nhan"){
        // Cong nhan tinh luong theo san pham
        baseSalary = productionService.calculateWorkerProductionAmount(NV_ID, month, year);

        // Cong nhan khong su dung he so luong
        salaryCoefficient = 0;

        if(baseSalary <= 0){
            message = "Nhan vien chua co san luong trong ky nay!";
            return false;
        }
    }
    else if(employeeType == "Van phong"){
        // Nhan vien van phong tinh theo luong co ban x he so
        if(baseSalary <= 0){
            message = "Luong co ban phai lon hon 0!";
            return false;
        }

        if(salaryCoefficient <= 0){
            message = "He so luong cua chuc vu khong hop le!";
            return false;
        }
    }
    else{
        message = "Loai nhan vien khong hop le!";
        return false;
    }

    // 6. Tinh tong luong
    double totalSalary = calculateTotalSalary(employeeType, baseSalary, salaryCoefficient, allowance, bonus, deduction);

    if(totalSalary < 0){
        message = "Khong the tinh luong cho nhan vien!";
        return false;
    }

    // 7. Tao doi tuong Salary
    Salary salary(generateSalaryID(), NV_ID, month, year, salaryCoefficient, baseSalary, allowance, bonus, deduction, totalSalary);

    // 8. Luu vao file
    if(!salaryRepository.add(salary)){
        message = "Luu bang luong that bai!";
        return false;
    }

    message = "Lap bang luong thanh cong!";
    return true;
}