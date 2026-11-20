#ifndef LEAVE_REQUEST_H
#define LEAVE_REQUEST_H

#include <string>
using namespace std;

class LeaveRequest {
private:
    string requestID;
    string NV_ID;

    string startDate;
    string endDate;
    string reason;

    string status;
    string managerNote;

public:
    LeaveRequest();

    LeaveRequest(string requestID, string NV_ID, string startDate, string endDate, string reason, string status, string managerNote);

    string getRequestID() const;
    string getNV_ID() const;

    string getStartDate() const;
    string getEndDate() const;
    string getReason() const;

    string getStatus() const;
    string getManagerNote() const;

    void setRequestID(string requestID);
    void setNV_ID(string NV_ID);

    void setStartDate(string startDate);
    void setEndDate(string endDate);
    void setReason(string reason);

    void setStatus(string status);
    void setManagerNote(string managerNote);
};

#endif