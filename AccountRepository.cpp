#include "AccountRepository.h"
#include <fstream>
#include <sstream>

AccountRepository::AccountRepository(string fileName){
    this->fileName = fileName;
}

// Doc danh sach tai khoan

vector<Account> AccountRepository::getAll(){
    vector<Account> accounts;
    ifstream file(fileName);

    if(!file.is_open()){
        return accounts;
    }

    string line;

    while(getline(file, line)){
        if(line.empty()){
            continue;
        }

        stringstream ss(line);

        string pass;
        string role;
        string NV_ID;
        string locked;

        getline(ss, pass, ',');
        getline(ss, role, ',');
        getline(ss, NV_ID, ',');
        getline(ss, locked, ',');

        // Bo qua dong tieu de va dong khong hop le
        if(pass.empty() || role.empty() || NV_ID.empty() || (locked != "0" && locked != "1" && locked != "true" && locked != "false")){
            continue;
        }

        bool isLocked = (locked == "1" || locked == "true");

        Account account(pass, role, NV_ID, isLocked);
        accounts.push_back(account);
    }
    
    file.close();
    return accounts;
}


// Ghi danh sach tai khoan
bool AccountRepository::saveAll(const vector<Account>& accounts){
    ofstream file(fileName);

    if(!file.is_open()){
        return false;
    }

    file << "Account" << endl;

    for(const Account& account : accounts){
        file << account.getPass() << "," << account.getRole() << "," << account.getNV_ID() << "," << (account.getIsLocked() ? "1" : "0") << endl;
    }

    file.close();
    return true;
}

// Them tai khoan
bool AccountRepository::add(const Account& account){
    vector<Account> accounts = getAll();

    // Moi nhan vien chi co mot tai khoan
    for(const Account& current : accounts){
        if(current.getNV_ID() == account.getNV_ID()){
            return false;
        }
    }

    accounts.push_back(account);
    return saveAll(accounts);
}

// Cap nhat tai khoan
bool AccountRepository::update(const Account& account){
    vector<Account> accounts = getAll();
    bool found = false;

    for(Account& current : accounts){
        if(current.getNV_ID() == account.getNV_ID()){
            current = account;
            found = true;
            break;
        }
    }

    if(!found){
        return false;
    }

    return saveAll(accounts);
}

// Xoa tai khoan theo ma nhan vien
bool AccountRepository::remove(string NV_ID){
    vector<Account> accounts = getAll();

    for(int i = 0; i < (int)accounts.size(); i++){
        if(accounts[i].getNV_ID() == NV_ID){
            accounts.erase(accounts.begin() + i);
            return saveAll(accounts);
        }
    }
    return false;
}

// Tim tai khoan theo ma nhan vien
bool AccountRepository::findByID(string NV_ID, Account& account){
    vector<Account> accounts = getAll();

    for(const Account& current : accounts){
        if(current.getNV_ID() == NV_ID){
            account = current;
            return true;
        }
    }
    return false;
}