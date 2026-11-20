#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
using namespace std;

class Product{
private:
    string productID;
    string productName;
    double unitPrice;
    bool status;

public:
    Product();
    Product(string productID, string productName,
            double unitPrice, bool status);

    string getProductID() const;
    string getProductName() const;
    double getUnitPrice() const;
    bool getStatus() const;

    void setProductID(string productID);
    void setProductName(string productName);
    void setUnitPrice(double unitPrice);
    void setStatus(bool status);
};

#endif
