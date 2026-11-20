#include "PositionService.h"

PositionService::PositionService(){
}

// Lay toan bo chuc vu
vector<Position> PositionService::getAllPositions(){
    return positionRepository.getAll();
}

// Lay chuc vu theo phong ban
vector<Position> PositionService::getPositionsByDepartment(string PHG_ID){
    vector<Position> result;
    vector<Position> positions = positionRepository.getAll();

    for(const Position& position : positions){
        if(position.getPHG_ID() == PHG_ID){
            result.push_back(position);
        }
    }
    return result;
}

// Them chuc vu
bool PositionService::addPosition(const Position& position){
    // Kiem tra phong ban ton tai
    Department department;

    if(!departmentRepository.findByID(position.getPHG_ID(), department)){
        return false;
    }

    // Kiem tra ten chuc vu
    if(position.getPositionName().empty()){
        return false;
    }

    // Kiem tra nhom nhan vien
    if(position.getEmployeeType().empty()){
        return false;
    }

    // Kiem tra he so luong
    if(position.getSalaryCoefficient() <= 0){
        return false;
    }

    // Kiem tra trung ten trong cung phong ban
    Position existingPosition;

    if(positionRepository.findByName(position.getPHG_ID(), position.getPositionName(), existingPosition)){
        return false;
    }
    return positionRepository.add(position);
}

// Cap nhat chuc vu
bool PositionService::updatePosition(string PHG_ID, string oldPositionName, const Position& newPosition){
    // Khong cho chuyen chuc vu sang phong ban khac
    if(newPosition.getPHG_ID() != PHG_ID){
        return false;
    }

    // Kiem tra chuc vu cu ton tai
    Position oldPosition;

    if(!positionRepository.findByName(PHG_ID, oldPositionName, oldPosition)){
        return false;
    }

    // Kiem tra thong tin moi
    if(newPosition.getPositionName().empty() || newPosition.getEmployeeType().empty() || newPosition.getSalaryCoefficient() <= 0){
        return false;
    }

    // Neu doi ten, kiem tra ten moi co bi trung khong
    if(oldPositionName != newPosition.getPositionName()){
        Position existingPosition;

        if(positionRepository.findByName(PHG_ID, newPosition.getPositionName(), existingPosition)){
            return false;
        }
    }

    // Lay danh sach nhan vien
    vector<Employee> employees = employeeRepository.getAll();
    vector<Employee> updatedEmployees = employees;

    // Cap nhat ten chuc vu cho nhan vien dang su dung chuc vu cu
    if(oldPositionName != newPosition.getPositionName()){
        for(Employee& employee : updatedEmployees){
            if(employee.getPHG_ID() == PHG_ID && employee.getPosition() == oldPositionName){
                employee.setPosition(newPosition.getPositionName());
            }
        }
    }

    // Cap nhat danh muc chuc vu
    if(!positionRepository.update(PHG_ID, oldPositionName, newPosition)){
        return false;
    }

    // Luu lai nhan vien neu co thay doi ten chuc vu
    if(oldPositionName != newPosition.getPositionName()){
        if(!employeeRepository.saveAll(updatedEmployees)){
            // Khoi phuc danh muc chuc vu neu luu nhan vien that bai
            positionRepository.update(PHG_ID, newPosition.getPositionName(), oldPosition);

            return false;
        }
    }
    return true;
}

// Xoa chuc vu
bool PositionService::deletePosition(string PHG_ID, string positionName){
    // Kiem tra chuc vu ton tai
    Position position;

    if(!positionRepository.findByName(PHG_ID, positionName, position)){
        return false;
    }

    // Khong cho xoa neu co nhan vien dang giu chuc vu nay
    vector<Employee> employees = employeeRepository.getAll();

    for(const Employee& employee : employees){
        if(employee.getPHG_ID() == PHG_ID && employee.getPosition() == positionName){
            return false;
        }
    }
    return positionRepository.remove(PHG_ID, positionName);
}

// Tim chuc vu
bool PositionService::findPosition(string PHG_ID, string positionName, Position& position){
    return positionRepository.findByName(PHG_ID, positionName, position);
}

