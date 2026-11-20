#include "ProductionRepository.h"
#include <fstream>
#include <sstream>
#include <iomanip>

ProductionRepository::ProductionRepository(string fileName){
    this->fileName = fileName;
}

vector<ProductionRecord> ProductionRepository::getAll(){
    vector<ProductionRecord> records;
    ifstream file(fileName);

    if(!file.is_open()){
        return records;
    }

    string line;

    while(getline(file, line)){
        if(line.empty()) continue;

        stringstream ss(line);

        string productionID;
        string NV_ID;
        string productID;
        string quantityText;
        string unitPriceText;
        string monthText;
        string yearText;

        getline(ss, productionID, ',');
        getline(ss, NV_ID, ',');
        getline(ss, productID, ',');
        getline(ss, quantityText, ',');
        getline(ss, unitPriceText, ',');
        getline(ss, monthText, ',');
        getline(ss, yearText, ',');

        if(productionID.empty() || productionID == "ProductionRecord"){
            continue;
        }

        try{
            int quantity = stoi(quantityText);
            double unitPrice = stod(unitPriceText);
            int month = stoi(monthText);
            int year = stoi(yearText);

            ProductionRecord record(productionID, NV_ID, productID, quantity, unitPrice, month, year);

            records.push_back(record);
        }
        catch(...){
            continue;
        }
    }

    file.close();
    return records;
}

bool ProductionRepository::saveAll(const vector<ProductionRecord>& records){
    ofstream file(fileName);

    if(!file.is_open()){
        return false;
    }

    file << "ProductionRecord" << endl;
    file << fixed << setprecision(2);

    for(const ProductionRecord& record : records){
        file << record.getProductionID() << ","
             << record.getNV_ID() << ","
             << record.getProductID() << ","
             << record.getQuantity() << ","
             << record.getUnitPrice() << ","
             << record.getMonth() << ","
             << record.getYear() << endl;
    }

    file.close();
    return true;
}

bool ProductionRepository::add(const ProductionRecord& record){
    vector<ProductionRecord> records = getAll();

    // Kiem tra trung ma san luong
    for(const ProductionRecord& current : records){
        if(current.getProductionID() == record.getProductionID()){
            return false;
        }
    }

    // Moi nhan vien chi co mot ban ghi
    // cho mot san pham trong mot ky luong
    for(const ProductionRecord& current : records){
        if(current.getNV_ID() == record.getNV_ID() && current.getProductID() == record.getProductID() && current.getMonth() == record.getMonth() && current.getYear() == record.getYear()){
            return false;
        }
    }

    records.push_back(record);
    return saveAll(records);
}

bool ProductionRepository::update(const ProductionRecord& record){
    vector<ProductionRecord> records = getAll();

    int index = -1;

    // Tim ban ghi can cap nhat
    for(int i = 0; i < (int)records.size(); i++){
        if(records[i].getProductionID() == record.getProductionID()){
            index = i;
            break;
        }
    }

    if(index == -1){
        return false;
    }

    // Kiem tra trung khoa ket hop
    for(int i = 0; i < (int)records.size(); i++){
        if(i == index) continue;

        if(records[i].getNV_ID() == record.getNV_ID() && records[i].getProductID() == record.getProductID() && records[i].getMonth() == record.getMonth() && records[i].getYear() == record.getYear()){
            return false;
        }
    }

    records[index] = record;

    return saveAll(records);
}

bool ProductionRepository::remove(string productionID){
    vector<ProductionRecord> records = getAll();

    for(int i = 0; i < (int)records.size(); i++){
        if(records[i].getProductionID() == productionID){
            records.erase(records.begin() + i);
            return saveAll(records);
        }
    }
    return false;
}

bool ProductionRepository::findByID(string productionID, ProductionRecord& record){
    vector<ProductionRecord> records = getAll();

    for(const ProductionRecord& current : records){
        if(current.getProductionID() == productionID){
            record = current;
            return true;
        }
    }
    return false;
}

vector<ProductionRecord> ProductionRepository::getByEmployeeAndMonth(string NV_ID, int month, int year){
    vector<ProductionRecord> records = getAll();
    vector<ProductionRecord> result;

    for(const ProductionRecord& record : records){
        if(record.getNV_ID() == NV_ID && record.getMonth() == month && record.getYear() == year){
            result.push_back(record);
        }
    }
    return result;
}

bool ProductionRepository::findByEmployeeProductMonth(string NV_ID, string productID, int month, int year, ProductionRecord& record){
    vector<ProductionRecord> records = getAll();

    for(const ProductionRecord& current : records){
        if(current.getNV_ID() == NV_ID && current.getProductID() == productID && current.getMonth() == month && current.getYear() == year){
            record = current;
            return true;
        }
    }
    return false;
}