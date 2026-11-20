#ifndef PRODUCTION_REPOSITORY_H
#define PRODUCTION_REPOSITORY_H

#include "../Models/ProductionRecord.h"
#include <vector>
#include <string>
using namespace std;

class ProductionRepository{
private:
    string fileName;

public:
    ProductionRepository(string fileName = "Data/ProductionRecord.txt");

    vector<ProductionRecord> getAll();

    bool saveAll(const vector<ProductionRecord>& records);

    bool add(const ProductionRecord& record);
    bool update(const ProductionRecord& record);
    bool remove(string productionID);

    bool findByID(string productionID, ProductionRecord& record);

    vector<ProductionRecord> getByEmployeeAndMonth(string NV_ID, int month, int year);

    bool findByEmployeeProductMonth(string NV_ID, string productID, int month, int year, ProductionRecord& record);
};

#endif
