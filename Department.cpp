#include "Department.h"

Department::Department(){
    PHG_ID = "";
    departmentName = "";
}

Department::Department(string PHG_ID, string departmentName){
    this->PHG_ID = PHG_ID;
    this->departmentName = departmentName;
}

string Department::getPHG_ID() const{
    return PHG_ID;
}

string Department::getDepartmentName() const{
    return departmentName;
}

void Department::setPHG_ID(string PHG_ID){
    this->PHG_ID = PHG_ID;
}

void Department::setDepartmentName(string departmentName){
    this->departmentName = departmentName;
}