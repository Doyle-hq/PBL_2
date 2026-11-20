#include "ProductionRecord.h"

ProductionRecord::ProductionRecord(){
    productionID = "";
    NV_ID = "";
    productID = "";

    quantity = 0;
    unitPrice = 0.0;

    month = 0;
    year = 0;
}

ProductionRecord::ProductionRecord(string productionID, string NV_ID, string productID, int quantity, double unitPrice, int month, int year){
    this->productionID = productionID;
    this->NV_ID = NV_ID;
    this->productID = productID;

    this->quantity = quantity;
    this->unitPrice = unitPrice;

    this->month = month;
    this->year = year;
}

string ProductionRecord::getProductionID() const{
    return productionID;
}

string ProductionRecord::getNV_ID() const{
    return NV_ID;
}

string ProductionRecord::getProductID() const{
    return productID;
}

int ProductionRecord::getQuantity() const{
    return quantity;
}

double ProductionRecord::getUnitPrice() const{
    return unitPrice;
}

int ProductionRecord::getMonth() const{
    return month;
}

int ProductionRecord::getYear() const{
    return year;
}

void ProductionRecord::setProductionID(string productionID){
    this->productionID = productionID;
}

void ProductionRecord::setNV_ID(string NV_ID){
    this->NV_ID = NV_ID;
}

void ProductionRecord::setProductID(string productID){
    this->productID = productID;
}

void ProductionRecord::setQuantity(int quantity){
    this->quantity = quantity;
}

void ProductionRecord::setUnitPrice(double unitPrice){
    this->unitPrice = unitPrice;
}

void ProductionRecord::setMonth(int month){
    this->month = month;
}

void ProductionRecord::setYear(int year){
    this->year = year;
}