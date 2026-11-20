#include "EmployeeRepository.h"
#include <fstream>
#include <sstream>

EmployeeRepository::EmployeeRepository(string fileName){
    this->fileName = fileName;
}

// Doc toan bo danh sach nhan vien tu file
vector<Employee> EmployeeRepository::getAll(){
    vector<Employee> employees;
    ifstream file(fileName);

    if(!file.is_open()){
        return employees;
    }

    string line;

    while(getline(file, line)){
        if(line.empty()){
            continue;
        }

        stringstream ss(line);

        string NV_ID;
        string fullName;
        string dateOfBirth;
        string gender;
        string phone;
        string address;
        string PHG_ID;
        string ngayVaoLam;
        string status;
        string position;

        getline(ss, NV_ID, ',');
        getline(ss, fullName, ',');
        getline(ss, dateOfBirth, ',');
        getline(ss, gender, ',');
        getline(ss, phone, ',');
        getline(ss, address, ',');
        getline(ss, PHG_ID, ',');
        getline(ss, ngayVaoLam, ',');
        getline(ss, status, ',');

        // Ho tro ca file cu 9 cot va file moi 10 cot
        getline(ss, position, ',');

        // Bo qua dong tieu de hoac dong khong hop le
        if(NV_ID.empty() || NV_ID == "Employee"){
            continue;
        }

        Employee employee(NV_ID, fullName, dateOfBirth, gender, phone, address, PHG_ID, ngayVaoLam, status, position);

        employees.push_back(employee);
    }

    file.close();
    return employees;
}

// Ghi lai toan bo danh sach nhan vien
bool EmployeeRepository::saveAll(const vector<Employee>& employees){
    ofstream file(fileName);

    if(!file.is_open()){
        return false;
    }

    file << "Employee" << endl;

    for(const Employee& employee : employees){
        file << employee.getNV_ID() << ","
             << employee.getFullName() << ","
             << employee.getDateOfBirth() << ","
             << employee.getGender() << ","
             << employee.getPhone() << ","
             << employee.getAddress() << ","
             << employee.getPHG_ID() << ","
             << employee.getNgayVaoLam() << ","
             << employee.getStatus() << ","
             << employee.getPosition() << endl;
    }

    file.close();
    return true;
}

// Them nhan vien
bool EmployeeRepository::add(const Employee& employee){
    vector<Employee> employees = getAll();

    // Kiem tra trung ma nhan vien
    for(const Employee& current : employees){
        if(current.getNV_ID() == employee.getNV_ID()){
            return false;
        }
    }

    employees.push_back(employee);
    return saveAll(employees);
}

// Cap nhat thong tin nhan vien
bool EmployeeRepository::update(const Employee& employee){
    vector<Employee> employees = getAll();
    bool found = false;

    for(Employee& current : employees){
        if(current.getNV_ID() == employee.getNV_ID()){
            current = employee;
            found = true;
            break;
        }
    }

    if(!found){
        return false;
    }

    return saveAll(employees);
}

// Xoa nhan vien theo ma
bool EmployeeRepository::remove(string NV_ID){
    vector<Employee> employees = getAll();

    for(int i = 0; i < (int)employees.size(); i++){
        if(employees[i].getNV_ID() == NV_ID){
            employees.erase(employees.begin() + i);
            return saveAll(employees);
        }
    }
    return false;
}

// Tim nhan vien theo ma
bool EmployeeRepository::findByID(string NV_ID, Employee& employee){
    vector<Employee> employees = getAll();

    for(const Employee& current : employees){
        if(current.getNV_ID() == NV_ID){
            employee = current;
            return true;
        }
    }
    return false;
}