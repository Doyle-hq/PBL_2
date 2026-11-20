#include "DepartmentService.h"

#include <sstream>
#include <iomanip>

// Khoi tao cac Repository
DepartmentService::DepartmentService(){
    departmentRepository = DepartmentRepository("Data/Department.txt");

    departmentIDCounterRepository = DepartmentIDCounterRepository("Data/DepartmentIDCounter.txt");

    employeeRepository = EmployeeRepository("Data/Employee.txt");

    positionRepository = PositionRepository("Data/Position.txt");
}

// Lay danh sach phong ban
vector<Department> DepartmentService::getAllDepartments(){
    return departmentRepository.getAll();
}

// Tim phong ban theo ma
bool DepartmentService::findByID(string PHG_ID, Department& department){
    return departmentRepository.findByID(PHG_ID, department);
}

// Tu dong sinh ma phong ban
string DepartmentService::generateDepartmentID(){
    vector<Department> departments = departmentRepository.getAll();

    int currentMax = 0;

    for(const Department& department : departments){
        try{
            int id = stoi(department.getPHG_ID());

            if(id > currentMax){
                currentMax = id;
            }
        }
        catch(...){
            continue;
        }
    }

    int nextNumber = departmentIDCounterRepository.getNextNumber(currentMax);

    if(nextNumber == 0){
        return "";
    }

    stringstream ss;
    ss << setw(3) << setfill('0') << nextNumber;

    return ss.str();
}

// Them phong ban
bool DepartmentService::addDepartment(string departmentName, string& message){
    if(departmentName.empty()){
        message = "Ten phong ban khong duoc de trong!";
        return false;
    }

    vector<Department> departments = departmentRepository.getAll();

    // Kiem tra trung ten phong ban
    for(const Department& department : departments){
        if(department.getDepartmentName() == departmentName){
            message = "Ten phong ban da ton tai!";
            return false;
        }
    }

    string newID = generateDepartmentID();

    if(newID.empty()){
        message = "Khong the tao ma phong ban!";
        return false;
    }

    Department department(newID, departmentName);

    if(!departmentRepository.add(department)){
        message = "Them phong ban that bai!";
        return false;
    }

    message = "Them phong ban thanh cong!";
    return true;
}

// Cap nhat ten phong ban
bool DepartmentService::updateDepartment(string PHG_ID, string newDepartmentName, string& message){
    if(newDepartmentName.empty()){
        message = "Ten phong ban khong duoc de trong!";
        return false;
    }

    Department department;

    if(!departmentRepository.findByID(PHG_ID, department)){
        message = "Khong tim thay phong ban!";
        return false;
    }

    vector<Department> departments = departmentRepository.getAll();

    // Kiem tra trung ten voi phong ban khac
    for(const Department& current : departments){
        if(current.getPHG_ID() != PHG_ID && current.getDepartmentName() == newDepartmentName){

            message = "Ten phong ban da duoc su dung!";
            return false;
        }
    }

    // Chi thay doi ten, giu nguyen ma phong ban
    department.setDepartmentName(newDepartmentName);

    if(!departmentRepository.update(department)){
        message = "Cap nhat phong ban that bai!";
        return false;
    }

    message = "Cap nhat phong ban thanh cong!";
    return true;
}

// Xoa phong ban
bool DepartmentService::deleteDepartment(string PHG_ID, string& message){
    Department department;

    if(!departmentRepository.findByID(PHG_ID, department)){
        message = "Khong tim thay phong ban!";
        return false;
    }

    // Kiem tra nhan vien thuoc phong ban
    vector<Employee> employees = employeeRepository.getAll();

    for(const Employee& employee : employees){
        if(employee.getPHG_ID() == PHG_ID){
            message = "Khong the xoa phong ban vi van con nhan vien!";
            return false;
        }
    }

    // Kiem tra chuc vu thuoc phong ban
    vector<Position> positions = positionRepository.getAll();

    for(const Position& position : positions){
        if(position.getPHG_ID() == PHG_ID){
            message = "Khong the xoa phong ban vi van con chuc vu!";
            return false;
        }
    }

    if(!departmentRepository.remove(PHG_ID)){
        message = "Xoa phong ban that bai!";
        return false;
    }

    message = "Xoa phong ban thanh cong!";
    return true;
}