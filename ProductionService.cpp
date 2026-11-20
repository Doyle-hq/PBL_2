#include "ProductionService.h"

#include <sstream>
#include <iomanip>
#include <ctime>

ProductionService::ProductionService(){
    productionRepository = ProductionRepository("Data/ProductionRecord.txt");
    productRepository = ProductRepository("Data/Product.txt");
    employeeRepository = EmployeeRepository("Data/Employee.txt");
    positionRepository = PositionRepository("Data/Position.txt");
    salaryRepository = SalaryRepository("Data/Salary.txt");
}

string ProductionService::generateProductionID(){
    vector<ProductionRecord> records = productionRepository.getAll();

    int maxNumber = 0;

    for(const ProductionRecord& record : records){
        string id = record.getProductionID();

        if(id.length() > 2 && id.substr(0, 2) == "PR"){
            try{
                int number = stoi(id.substr(2));

                if(number > maxNumber){
                    maxNumber = number;
                }
            }
            catch(...){
                continue;
            }
        }
    }

    stringstream ss;
    ss << "PR" << setw(6) << setfill('0') << maxNumber + 1;

    return ss.str();
}

vector<ProductionRecord> ProductionService::getAllRecords(){
    return productionRepository.getAll();
}

vector<ProductionRecord> ProductionService::getEmployeeProduction(string NV_ID, int month, int year){
    return productionRepository.getByEmployeeAndMonth(NV_ID, month, year);
}

bool ProductionService::addProduction(string NV_ID, string productID, int quantity, int month, int year, string& message){
    // 1. Kiem tra thong tin dau vao
    if(NV_ID.empty() || productID.empty()){
        message = "Thong tin nhan vien hoac san pham khong hop le!";
        return false;
    }

    if(quantity <= 0){
        message = "So luong san pham phai lon hon 0!";
        return false;
    }

    if(month < 1 || month > 12){
        message = "Thang khong hop le!";
        return false;
    }

    if(year < 2000){
        message = "Nam khong hop le!";
        return false;
    }

    // Khong cho phep nhap san luong neu ky luong da duoc lap
    if(isSalaryCreated(NV_ID, month, year)){
        message = "Ky luong da duoc lap, khong the nhap them san luong!";
        return false;
    }

    // 2. Kiem tra nhan vien
    Employee employee;

    if(!employeeRepository.findByID(NV_ID, employee)){
        message = "Khong tim thay nhan vien!";
        return false;
    }

    if(employee.getStatus() != "Dang lam"){
        message = "Nhan vien khong con lam viec!";
        return false;
    }

    // 3. Kiem tra loai nhan vien thong qua chuc vu
    vector<Position> positions = positionRepository.getAll();

    bool isWorker = false;

    for(const Position& position : positions){
        if(position.getPHG_ID() == employee.getPHG_ID() && position.getPositionName() == employee.getPosition()){

            if(position.getEmployeeType() == "Cong nhan"){
                isWorker = true;
            }

            break;
        }
    }

    if(!isWorker){
        message = "Chi cong nhan moi duoc nhap san luong!";
        return false;
    }

    // 4. Kiem tra san pham
    Product product;

    if(!productRepository.findByID(productID, product)){
        message = "Khong tim thay san pham!";
        return false;
    }

    if(!product.getStatus()){
        message = "San pham da ngung kinh doanh!";
        return false;
    }

    if(product.getUnitPrice() <= 0){
        message = "Don gia san pham khong hop le!";
        return false;
    }

    // 5. Kiem tra ban ghi da ton tai chua
    ProductionRecord oldRecord;

    if(productionRepository.findByEmployeeProductMonth(NV_ID, productID, month, year, oldRecord)){
        // Neu da co thi cong don so luong
        oldRecord.setQuantity(oldRecord.getQuantity() + quantity);

        if(!productionRepository.update(oldRecord)){
            message = "Khong the cap nhat san luong!";
            return false;
        }

        message = "Da cong don san luong thanh cong!";
        return true;
    }

    // 6. Tao ban ghi moi
    ProductionRecord record(generateProductionID(), NV_ID, productID, quantity, product.getUnitPrice(), month, year);

    if(!productionRepository.add(record)){
        message = "Khong the luu san luong!";
        return false;
    }

    message = "Nhap san luong thanh cong!";
    return true;
}

bool ProductionService::updateProduction(string productionID, int quantity, string& message){
    if(quantity <= 0){
        message = "So luong phai lon hon 0!";
        return false;
    }

    ProductionRecord record;

    if(!productionRepository.findByID(productionID, record)){
        message = "Khong tim thay ban ghi san luong!";
        return false;
    }

    if(isSalaryCreated(record.getNV_ID(), record.getMonth(), record.getYear())){
    message = "Ky luong da duoc lap, khong the sua san luong!";
    return false;
    }

    record.setQuantity(quantity);

    if(!productionRepository.update(record)){
        message = "Cap nhat san luong that bai!";
        return false;
    }

    message = "Cap nhat san luong thanh cong!";
    return true;
}

bool ProductionService::deleteProduction(string productionID, string& message){
    ProductionRecord record;

    if(!productionRepository.findByID(productionID, record)){
        message = "Khong tim thay ban ghi san luong!";
        return false;
    }

    if(isSalaryCreated(record.getNV_ID(), record.getMonth(), record.getYear())){
        message = "Ky luong da duoc lap, khong the xoa san luong!";
        return false;
    }

    if(!productionRepository.remove(productionID)){
        message = "Xoa san luong that bai!";
        return false;
    }

    message = "Xoa san luong thanh cong!";
    return true;
}

double ProductionService::calculateWorkerProductionAmount(string NV_ID, int month, int year){
    vector<ProductionRecord> records = productionRepository.getByEmployeeAndMonth(NV_ID, month, year);

    double total = 0;

    for(const ProductionRecord& record : records){
        total += record.getQuantity() * record.getUnitPrice();
    }
    return total;
}

bool ProductionService::findProductionByID(string productionID, ProductionRecord& record){
    return productionRepository.findByID(productionID, record);
}

bool ProductionService::isSalaryCreated(string NV_ID, int month, int year){
    vector<Salary> salaries = salaryRepository.getAll();

    for(const Salary& salary : salaries){
        if(salary.getNV_ID() == NV_ID && salary.getMonth() == month && salary.getYear() == year){
            return true;
        }
    }
    return false;
}