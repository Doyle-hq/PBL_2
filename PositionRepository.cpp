#include "PositionRepository.h"
#include <fstream>
#include <sstream>

PositionRepository::PositionRepository(string fileName){
    this->fileName = fileName;
}

vector<Position> PositionRepository::getAll(){
    vector<Position> positions;
    ifstream file(fileName);

    if(!file.is_open()){
        return positions;
    }

    string line;

    while(getline(file, line)){
        if(line.empty()){
            continue;
        }

        stringstream ss(line);

        string PHG_ID;
        string positionName;
        string employeeType;
        string coefficientText;

        getline(ss, PHG_ID, ',');
        getline(ss, positionName, ',');
        getline(ss, employeeType, ',');
        getline(ss, coefficientText, ',');

        if(PHG_ID.empty() || PHG_ID == "Position"){
            continue;
        }

        try{
            double coefficient = stod(coefficientText);

            Position position(PHG_ID, positionName, employeeType, coefficient);

            positions.push_back(position);
        }
        catch(...){
            continue;
        }
    }

    file.close();
    return positions;
}

bool PositionRepository::saveAll(const vector<Position>& positions){
    ofstream file(fileName);

    if(!file.is_open()){
        return false;
    }

    file << "Position" << endl;

    for(const Position& position : positions){
        file << position.getPHG_ID() << ","
             << position.getPositionName() << ","
             << position.getEmployeeType() << ","
             << position.getSalaryCoefficient() << endl;
    }

    file.close();
    return true;
}

bool PositionRepository::add(const Position& position){
    vector<Position> positions = getAll();

    for(const Position& current : positions){
        if(current.getPHG_ID() == position.getPHG_ID() && current.getPositionName() == position.getPositionName()){
            return false;
        }
    }

    positions.push_back(position);
    return saveAll(positions);
}

bool PositionRepository::update(string oldPHG_ID, string oldPositionName, const Position& newPosition){
    vector<Position> positions = getAll();

    bool found = false;

    // Kiem tra chuc vu cu co ton tai khong
    for(const Position& current : positions){
        if(current.getPHG_ID() == oldPHG_ID && current.getPositionName() == oldPositionName){
            found = true;
            break;
        }
    }

    if(!found){
        return false;
    }

    // Kiem tra trung ten chuc vu trong cung phong ban
    for(const Position& current : positions){
        if(current.getPHG_ID() == newPosition.getPHG_ID() && current.getPositionName() == newPosition.getPositionName()){
            // Bo qua chinh chuc vu dang duoc cap nhat
            if(current.getPHG_ID() != oldPHG_ID || current.getPositionName() != oldPositionName){
                return false;
            }
        }
    }

    // Cap nhat sau khi kiem tra hop le
    for(Position& current : positions){
        if(current.getPHG_ID() == oldPHG_ID && current.getPositionName() == oldPositionName){
            current = newPosition;
            break;
        }
    }
    return saveAll(positions);
}

bool PositionRepository::remove(string PHG_ID, string positionName){
    vector<Position> positions = getAll();

    for(int i = 0; i < (int)positions.size(); i++){
        if(positions[i].getPHG_ID() == PHG_ID && positions[i].getPositionName() == positionName){
            positions.erase(positions.begin() + i);
            return saveAll(positions);
        }
    }
    return false;
}

bool PositionRepository::findByName(string PHG_ID, string positionName, Position& position){
    vector<Position> positions = getAll();

    for(const Position& current : positions){
        if(current.getPHG_ID() == PHG_ID && current.getPositionName() == positionName){
            position = current;
            return true;
        }
    }
    return false;
}