#include "EmployeeIDCounterRepository.h"
#include <fstream>
#include <sstream>

EmployeeIDCounterRepository::EmployeeIDCounterRepository(string fileName){
    this->fileName = fileName;
}

int EmployeeIDCounterRepository::getNextNumber(string PHG_ID, int currentMax){
    vector<pair<string, int>> counters;
    ifstream inFile(fileName);

    string line;

    if(inFile.is_open()){
        while(getline(inFile, line)){
            if(line.empty() || line == "EmployeeIDCounter"){
                continue;
            }

            stringstream ss(line);
            string id;
            string numberText;

            getline(ss, id, ',');
            getline(ss, numberText, ',');

            if(id.empty() || numberText.empty()){
                continue;
            }

            try{
                int number = stoi(numberText);
                counters.push_back({id, number});
            } catch(...){
                continue;
            }
        }
        inFile.close();
    }

    int nextNumber = currentMax + 1;
    bool found = false;

    for(auto& counter : counters){
        if(counter.first == PHG_ID){
            if(counter.second > currentMax){
                currentMax = counter.second;
            }

            nextNumber = currentMax + 1;
            counter.second = nextNumber;
            found = true;
            break;
        }
    }

    if(!found){
        nextNumber = currentMax + 1;
        counters.push_back({PHG_ID, nextNumber});
    }

    ofstream outFile(fileName);

    if(!outFile.is_open()){
        return 0;
    }

    outFile << "EmployeeIDCounter" << endl;

    for(const auto& counter : counters){
        outFile << counter.first << ","
                << counter.second << endl;
    }

    outFile.close();

    return nextNumber;
}