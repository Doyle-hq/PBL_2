#include "DepartmentIDCounterRepository.h"
#include <fstream>
#include <string>

DepartmentIDCounterRepository::DepartmentIDCounterRepository(string fileName){
    this->fileName = fileName;
}

int DepartmentIDCounterRepository::getNextNumber(int currentMax){
    int lastNumber = 0;

    // Doc so phong ban cuoi cung da duoc cap
    ifstream inFile(fileName);

    if(inFile.is_open()){
        string header;
        getline(inFile, header);

        inFile >> lastNumber;
        inFile.close();
    }

    // Lay so lon hon giua file dem va du lieu hien tai
    if(currentMax > lastNumber){
        lastNumber = currentMax;
    }

    int nextNumber = lastNumber + 1;

    // Ma phong ban chi co 3 chu so
    if(nextNumber > 999){
        return 0;
    }

    // Luu lai ma vua cap
    ofstream outFile(fileName);

    if(!outFile.is_open()){
        return 0;
    }

    outFile << "DepartmentIDCounter" << endl;
    outFile << nextNumber << endl;

    outFile.close();

    return nextNumber;
}