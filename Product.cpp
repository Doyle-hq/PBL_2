#include "Product.h"

Product::Product(){
    productID = "";
    productName = "";
    unitPrice = 0.0;
    status = true;
}

Product::Product(string productID, string productName, double unitPrice, bool status){
    this->productID = productID;
    this->productName = productName;
    this->unitPrice = unitPrice;
    this->status = status;
}

string Product::getProductID() const{
    return productID;
}

string Product::getProductName() const{
    return productName;
}

double Product::getUnitPrice() const{
    return unitPrice;
}

bool Product::getStatus() const{
    return status;
}

void Product::setProductID(string productID){
    this->productID = productID;
}

void Product::setProductName(string productName){
    this->productName = productName;
}

void Product::setUnitPrice(double unitPrice){
    this->unitPrice = unitPrice;
}

void Product::setStatus(bool status){
    this->status = status;
}
