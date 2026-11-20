#include "EmployeeService.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cctype>

EmployeeService::EmployeeService() : employeeRepository("Data/Employee.txt"), accountRepository("Data/Account.txt"), positionRepository("Data/Position.txt"), employeeIDCounterRepository("Data/EmployeeIDCounter.txt"){
}

bool EmployeeService::validateEmployee(const Employee& employee, string& message){
    if(employee.getFullName().empty()){
        message = "Ho ten khong duoc de trong!";
        return false;
    }

    if(employee.getPHG_ID().length() != 3){
        message = "Ma phong ban phai co 3 ky tu!";
        return false;
    }

    for(char c : employee.getPHG_ID()){
        if(!isdigit(c)){
            message = "Ma phong ban phai la chu so!";
            return false;
        }
    }

    if(employee.getGender() != "Nam" && employee.getGender() != "Nu"){
        message = "Gioi tinh phai la Nam hoac Nu!";
        return false;
    }

    if(employee.getPhone().empty()){
        message = "So dien thoai khong duoc de trong!";
        return false;
    }
    return true;
}

string EmployeeService::getCurrentDate(){
    time_t now = time(0);
    tm* localTime = localtime(&now);

    stringstream ss;
    ss << put_time(localTime, "%d/%m/%Y");

    return ss.str();
}

string EmployeeService::generateEmployeeID(string PHG_ID){
    vector<Employee> employees = employeeRepository.getAll();

    int maxNumber = 0;

    // Tim ma so lon nhat hien co trong phong ban
    for(const Employee& employee : employees){
        string id = employee.getNV_ID();

        if(id.length() == 8 && id.substr(0, 3) == PHG_ID){
            try{
                int number = stoi(id.substr(3));

                if(number > maxNumber){
                    maxNumber = number;
                }
            }catch (...){
                continue;
            }
        }
    }

    // Lay so thu tu tiep theo tu file bo dem
    int nextNumber = employeeIDCounterRepository.getNextNumber(PHG_ID, maxNumber);

    if(nextNumber == 0 || nextNumber > 99999){
        return "";
    }

    stringstream ss;
    ss << PHG_ID << setw(5) << setfill('0') << nextNumber;

    return ss.str();
}

vector<Employee> EmployeeService::getAllEmployees(){
    return employeeRepository.getAll();
}

bool EmployeeService::prepareEmployee(Employee& employee, string& message){
    if(!validateEmployee(employee, message)){
        return false;
    }

    if(employee.getNV_ID().empty()){
        string newID = generateEmployeeID(employee.getPHG_ID());

        if(newID.empty()){
            message = "Khong the tao ma nhan vien!";
            return false;
        }
        employee.setNV_ID(newID);
    }

    if(employee.getNgayVaoLam().empty()){
        employee.setNgayVaoLam(getCurrentDate());
    }

    if(employee.getStatus().empty()){
        employee.setStatus("Dang lam");
    }

    message = "Du lieu nhan vien hop le!";
    return true;
}

bool EmployeeService::addEmployee(Employee& employee, string& message){
    if(!prepareEmployee(employee, message)){
        return false;
    }
    // Kiem tra chuc vu co ton tai trong dung phong ban khong
    Position position;

    if(!positionRepository.findByName(employee.getPHG_ID(), employee.getPosition(), position)){
        message = "Chuc vu khong ton tai trong phong ban!";
        return false;
    }

    Account account(employee.getNV_ID(), "employee", employee.getNV_ID(), false);

    if(!accountRepository.add(account)){
        message = "Khong the tao tai khoan nhan vien!";
        return false;
    }

    if(!employeeRepository.add(employee)){
        accountRepository.remove(employee.getNV_ID());
        message = "Khong the them nhan vien!";
        return false;
    }

    message = "Them nhan vien va tao tai khoan thanh cong!";
    return true;
}

bool EmployeeService::updateEmployee(const Employee& employee, string& message){
    Employee oldEmployee;

    if(!employeeRepository.findByID(employee.getNV_ID(), oldEmployee)){
        message = "Khong tim thay nhan vien!";
        return false;
    }

    if(oldEmployee.getStatus() == "Da nghi viec"){
        message = "Nhan vien da nghi viec, khong the cap nhat!";
        return false;
    }

    if(!validateEmployee(employee, message)){
        return false;
    }

    Position position;

    if(!positionRepository.findByName(employee.getPHG_ID(), employee.getPosition(), position)){
        message = "Chuc vu khong ton tai trong phong ban!";
        return false;
    }

    if(!employeeRepository.update(employee)){
        message = "Cap nhat that bai!";
        return false;
    }

    message = "Cap nhat nhan vien thanh cong!";
    return true;
}

bool EmployeeService::updateMyContactInfo(const string& NV_ID, const string& phone, const string& address, string& message){
    Employee employee;

    if(!employeeRepository.findByID(NV_ID, employee)){
        message = "Khong tim thay nhan vien!";
        return false;
    }

    if(employee.getStatus() == "Da nghi viec"){
        message = "Nhan vien da nghi viec!";
        return false;
    }

    if(phone.empty()){
        message = "So dien thoai khong duoc de trong!";
        return false;
    }

    if(address.empty()){
        message = "Dia chi khong duoc de trong!";
        return false;
    }

    employee.setPhone(phone);
    employee.setAddress(address);

    if(!employeeRepository.update(employee)){
        message = "Cap nhat ho so that bai!";
        return false;
    }

    message = "Cap nhat ho so ca nhan thanh cong!";
    return true;
}

bool EmployeeService::deleteEmployee(string NV_ID, string& message){
    Employee employee;

    if(!employeeRepository.findByID(NV_ID, employee)){
        message = "Khong tim thay nhan vien!";
        return false;
    }

    if(!employeeRepository.remove(NV_ID)){
        message = "Xoa nhan vien that bai!";
        return false;
    }

    accountRepository.remove(NV_ID);

    message = "Xoa nhan vien thanh cong!";
    return true;
}

bool EmployeeService::findByID(string NV_ID, Employee& employee){
    return employeeRepository.findByID(NV_ID, employee);
}

vector<Employee> EmployeeService::searchByName(string name){
    vector<Employee> employees = getAllEmployees();
    vector<Employee> result;

    for(const Employee& employee : employees){
        if(employee.getFullName().find(name) != string::npos){
            result.push_back(employee);
        }
    }
    return result;
}

vector<Employee> EmployeeService::sortByID(){
    vector<Employee> employees = getAllEmployees();

    sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b){
        return a.getNV_ID() < b.getNV_ID();
    });
    return employees;
}

vector<Employee> EmployeeService::sortByName(){
    vector<Employee> employees = getAllEmployees();

    sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b){
        return a.getFullName() < b.getFullName();
    });
    return employees;
}

vector<Employee> EmployeeService::sortByDepartment(){
    vector<Employee> employees = getAllEmployees();

    sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b){
        return a.getPHG_ID() < b.getPHG_ID();
    });

    return employees;
}

vector<Employee> EmployeeService::filterByDepartment(string PHG_ID){
    vector<Employee> employees = getAllEmployees();
    vector<Employee> result;

    for(const Employee& employee : employees){
        if(employee.getPHG_ID() == PHG_ID){
            result.push_back(employee);
        }
    }
    return result;
}

bool EmployeeService::confirmResignation(string NV_ID, string& message){
    Employee employee;

    // 1. Tim nhan vien
    if(!employeeRepository.findByID(NV_ID, employee)){
        message = "Khong tim thay nhan vien!";
        return false;
    }

    // 2. Kiem tra trang thai
    if(employee.getStatus() == "Da nghi viec"){
        message = "Nhan vien da nghi viec truoc do!";
        return false;
    }

    // 3. Tim tai khoan truoc khi thay doi du lieu
    Account account;

    if(!accountRepository.findByID(NV_ID, account)){
        message = "Khong tim thay tai khoan nhan vien!";
        return false;
    }

    // Luu lai trang thai khoa ban dau
    bool oldLockStatus = account.getIsLocked();

    // 4. Khoa tai khoan neu chua khoa
    if(!oldLockStatus){
        account.setIsLocked(true);

        if(!accountRepository.update(account)){
            message = "Khong the khoa tai khoan nhan vien!";
            return false;
        }
    }

    // 5. Cap nhat trang thai nhan vien
    employee.setStatus("Da nghi viec");

    if(!employeeRepository.update(employee)){
        // Khoi phuc trang thai tai khoan neu truoc do chua khoa
        if(!oldLockStatus){
            account.setIsLocked(false);
            accountRepository.update(account);
        }

        message = "Xac nhan nghi viec that bai!";
        return false;
    }

    message = "Da xac nhan nhan vien nghi viec va khoa tai khoan!";
    return true;
}