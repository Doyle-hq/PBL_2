#include "DepartmentRepository.h"
#include <fstream>
#include <sstream>

DepartmentRepository::DepartmentRepository(string fileName){
    this->fileName = fileName;
}

vector<Department> DepartmentRepository::getAll(){
    vector<Department> departments;
    ifstream file(fileName);

    if(!file.is_open()){
        return departments;
    }

    string line;

    while(getline(file, line)){
        if(line.empty()) continue;

        stringstream ss(line);
        string PHG_ID, departmentName;

        getline(ss, PHG_ID, ',');
        getline(ss, departmentName, ',');

        if(PHG_ID.empty() || PHG_ID == "Department"){
            continue;
        }

        Department department(PHG_ID, departmentName);
        departments.push_back(department);
    }

    file.close();
    return departments;
}

bool DepartmentRepository::saveAll(const vector<Department>& departments){
    ofstream file(fileName);

    if(!file.is_open()){
        return false;
    }

    file << "Department" << endl;

    for(const Department& department : departments){
        file << department.getPHG_ID() << ","
             << department.getDepartmentName() << endl;
    }

    file.close();
    return true;
}

bool DepartmentRepository::add(const Department& department){
    vector<Department> departments = getAll();

    for(const Department& current : departments){
        if(current.getPHG_ID() == department.getPHG_ID()){
            return false;
        }
    }

    departments.push_back(department);
    return saveAll(departments);
}

bool DepartmentRepository::update(const Department& department){
    vector<Department> departments = getAll();
    bool found = false;

    for(Department& current : departments){
        if(current.getPHG_ID() == department.getPHG_ID()){
            current = department;
            found = true;
            break;
        }
    }

    if(!found) return false;

    return saveAll(departments);
}

bool DepartmentRepository::remove(string PHG_ID){
    vector<Department> departments = getAll();

    for(int i = 0; i < (int)departments.size(); i++){
        if(departments[i].getPHG_ID() == PHG_ID){
            departments.erase(departments.begin() + i);
            return saveAll(departments);
        }
    }
    return false;
}

bool DepartmentRepository::findByID(string PHG_ID, Department& department){
    vector<Department> departments = getAll();

    for(const Department& current : departments){
        if(current.getPHG_ID() == PHG_ID){
            department = current;
            return true;
        }
    }
    return false;
}