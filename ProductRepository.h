#ifndef PRODUCT_REPOSITORY_H
#define PRODUCT_REPOSITORY_H

#include "../Models/Product.h"
#include <vector>
#include <string>
using namespace std;

class ProductRepository{
private:
    string fileName;

public:
    ProductRepository(string fileName = "Data/Product.txt");

    vector<Product> getAll();
    bool saveAll(const vector<Product>& products);

    bool add(const Product& product);
    bool update(const Product& product);
    bool remove(string productID);
    bool findByID(string productID, Product& product);
};

#endif
