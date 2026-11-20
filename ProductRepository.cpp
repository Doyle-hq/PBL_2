#include "ProductRepository.h"
#include <fstream>
#include <sstream>
#include <iomanip>

ProductRepository::ProductRepository(string fileName){
    this->fileName = fileName;
}

vector<Product> ProductRepository::getAll(){
    vector<Product> products;
    ifstream file(fileName);

    if(!file.is_open()){
        return products;
    }

    string line;

    while(getline(file, line)){
        if(line.empty()) continue;

        stringstream ss(line);

        string productID;
        string productName;
        string unitPriceText;
        string statusText;

        getline(ss, productID, ',');
        getline(ss, productName, ',');
        getline(ss, unitPriceText, ',');
        getline(ss, statusText, ',');

        if(productID.empty() || productID == "Product"){
            continue;
        }

        try{
            double unitPrice = stod(unitPriceText);

            bool status = (statusText == "1" || statusText == "true");

            Product product(productID, productName, unitPrice, status);

            products.push_back(product);
        }
        catch(...){
            continue;
        }
    }

    file.close();
    return products;
}

bool ProductRepository::saveAll(const vector<Product>& products){
    ofstream file(fileName);

    if(!file.is_open()){
        return false;
    }

    file << "Product" << endl;
    file << fixed << setprecision(2);

    for(const Product& product : products){
        file << product.getProductID() << "," << product.getProductName() << "," << product.getUnitPrice() << "," << (product.getStatus() ? 1 : 0) << endl;
    }

    file.close();
    return true;
}

bool ProductRepository::add(const Product& product){
    vector<Product> products = getAll();

    for(const Product& current : products){
        if(current.getProductID() == product.getProductID()){
            return false;
        }
    }

    products.push_back(product);
    return saveAll(products);
}

bool ProductRepository::update(const Product& product){
    vector<Product> products = getAll();

    for(Product& current : products){
        if(current.getProductID() == product.getProductID()){
            current = product;
            return saveAll(products);
        }
    }

    return false;
}

bool ProductRepository::remove(string productID){
    vector<Product> products = getAll();

    for(int i = 0; i < (int)products.size(); i++){
        if(products[i].getProductID() == productID){
            products.erase(products.begin() + i);
            return saveAll(products);
        }
    }

    return false;
}

bool ProductRepository::findByID(string productID, Product& product){
    vector<Product> products = getAll();

    for(const Product& current : products){
        if(current.getProductID() == productID){
            product = current;
            return true;
        }
    }

    return false;
}