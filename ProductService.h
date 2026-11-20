#ifndef PRODUCT_SERVICE_H
#define PRODUCT_SERVICE_H

#include "../Repositories/ProductRepository.h"
#include <vector>
#include <string>
using namespace std;

class ProductService{
private:
    ProductRepository productRepository;

public:
    ProductService();

    vector<Product> getAllProducts();
    vector<Product> getActiveProducts();

    bool addProduct(const Product& product);
    bool updateProduct(const Product& product);
    bool deactivateProduct(string productID);
    bool activateProduct(string productID);

    bool findByID(string productID, Product& product);
};

#endif
