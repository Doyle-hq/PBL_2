#include "ProductService.h"

ProductService::ProductService(){
    productRepository = ProductRepository("Data/Product.txt");
}

vector<Product> ProductService::getAllProducts(){
    return productRepository.getAll();
}

vector<Product> ProductService::getActiveProducts(){
    vector<Product> products = productRepository.getAll();
    vector<Product> result;

    for(const Product& product : products){
        if(product.getStatus()){
            result.push_back(product);
        }
    }

    return result;
}

bool ProductService::addProduct(const Product& product){
    // Kiem tra ma san pham
    if(product.getProductID().empty()){
        return false;
    }

    // Kiem tra ten san pham
    if(product.getProductName().empty()){
        return false;
    }

    // Kiem tra don gia
    if(product.getUnitPrice() <= 0){
        return false;
    }

    // San pham moi mac dinh dang kinh doanh
    Product newProduct(product.getProductID(), product.getProductName(), product.getUnitPrice(), true);

    return productRepository.add(newProduct);
}

bool ProductService::updateProduct(const Product& product){
    Product oldProduct;

    if(!productRepository.findByID(product.getProductID(), oldProduct)){
        return false;
    }

    if(product.getProductName().empty()){
        return false;
    }

    if(product.getUnitPrice() <= 0){
        return false;
    }

    Product updatedProduct(product.getProductID(), product.getProductName(), product.getUnitPrice(), oldProduct.getStatus());

    return productRepository.update(updatedProduct);
}

bool ProductService::deactivateProduct(string productID){
    Product product;

    if(!productRepository.findByID(productID, product)){
        return false;
    }

    if(!product.getStatus()){
        return false;
    }

    product.setStatus(false);

    return productRepository.update(product);
}

bool ProductService::activateProduct(string productID){
    Product product;

    if(!productRepository.findByID(productID, product)){
        return false;
    }

    if(product.getStatus()){
        return false;
    }

    product.setStatus(true);

    return productRepository.update(product);
}

bool ProductService::findByID(string productID, Product& product){
    return productRepository.findByID(productID, product);
}