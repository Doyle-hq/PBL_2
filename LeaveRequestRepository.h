#ifndef LEAVE_REQUEST_REPOSITORY_H
#define LEAVE_REQUEST_REPOSITORY_H

#include "../Models/LeaveRequest.h"
#include <vector>
#include <string>
using namespace std;

class LeaveRequestRepository{
private:
    string fileName;

public:
    LeaveRequestRepository(string fileName = "Data/LeaveRequest.txt");

    vector<LeaveRequest> getAll();
    bool saveAll(const vector<LeaveRequest>& requests);

    bool add(const LeaveRequest& request);
    bool update(const LeaveRequest& request);
    bool remove(string requestID);
    bool findByID(string requestID, LeaveRequest& request);
};

#endif
