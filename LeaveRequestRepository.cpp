#include "LeaveRequestRepository.h"
#include <fstream>
#include <sstream>

LeaveRequestRepository::LeaveRequestRepository(string fileName){
    this->fileName = fileName;
}

vector<LeaveRequest> LeaveRequestRepository::getAll(){
    vector<LeaveRequest> requests;
    ifstream file(fileName);

    if(!file.is_open()){
        return requests;
    }

    string line;

    while(getline(file, line)){
        if(line.empty()) continue;

        stringstream ss(line);

        string requestID;
        string NV_ID;
        string startDate;
        string endDate;
        string reason;
        string status;
        string managerNote;

        getline(ss, requestID, ',');
        getline(ss, NV_ID, ',');
        getline(ss, startDate, ',');
        getline(ss, endDate, ',');
        getline(ss, reason, ',');
        getline(ss, status, ',');
        getline(ss, managerNote, ',');

        if(requestID.empty() || requestID == "LeaveRequest"){
            continue;
        }

        LeaveRequest request(requestID, NV_ID, startDate, endDate, reason, status, managerNote);

        requests.push_back(request);
    }

    file.close();
    return requests;
}

bool LeaveRequestRepository::saveAll(const vector<LeaveRequest>& requests){
    ofstream file(fileName);

    if(!file.is_open()){
        return false;
    }

    file << "LeaveRequest" << endl;

    for(const LeaveRequest& request : requests){
        file << request.getRequestID() << "," << request.getNV_ID() << "," << request.getStartDate() << "," << request.getEndDate() << "," << request.getReason() << "," << request.getStatus() << "," << request.getManagerNote() << endl;
    }

    file.close();
    return true;
}

bool LeaveRequestRepository::add(const LeaveRequest& request){
    vector<LeaveRequest> requests = getAll();

    for(const LeaveRequest& current : requests){
        if(current.getRequestID() == request.getRequestID()){
            return false;
        }
    }

    requests.push_back(request);
    return saveAll(requests);
}

bool LeaveRequestRepository::update(const LeaveRequest& request){
    vector<LeaveRequest> requests = getAll();
    bool found = false;

    for(LeaveRequest& current : requests){
        if(current.getRequestID() == request.getRequestID()){
            current = request;
            found = true;
            break;
        }
    }

    if(!found) return false;

    return saveAll(requests);
}

bool LeaveRequestRepository::remove(string requestID){
    vector<LeaveRequest> requests = getAll();

    for(int i = 0; i < (int)requests.size(); i++){
        if(requests[i].getRequestID() == requestID){
            requests.erase(requests.begin() + i);
            return saveAll(requests);
        }
    }
    return false;
}

bool LeaveRequestRepository::findByID(string requestID, LeaveRequest& request){
    vector<LeaveRequest> requests = getAll();

    for(const LeaveRequest& current : requests){
        if(current.getRequestID() == requestID){
            request = current;
            return true;
        }
    }
    return false;
}