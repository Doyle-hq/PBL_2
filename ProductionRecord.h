#ifndef PRODUCTION_RECORD_H
#define PRODUCTION_RECORD_H

#include <string>
using namespace std;

class ProductionRecord{
private:
    string productionID;
    string NV_ID;
    string productID;

    int quantity;
    double unitPrice;

    int month;
    int year;

public:
    ProductionRecord();
    ProductionRecord(string productionID, string NV_ID, string productID, int quantity, double unitPrice, int month, int year);

    string getProductionID() const;
    string getNV_ID() const;
    string getProductID() const;

    int getQuantity() const;
    double getUnitPrice() const;

    int getMonth() const;
    int getYear() const;

    void setProductionID(string productionID);
    void setNV_ID(string NV_ID);
    void setProductID(string productID);

    void setQuantity(int quantity);
    void setUnitPrice(double unitPrice);

    void setMonth(int month);
    void setYear(int year);
};

#endif
