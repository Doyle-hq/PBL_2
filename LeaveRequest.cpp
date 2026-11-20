#include "LeaveRequest.h"

LeaveRequest::LeaveRequest(){
    requestID = "";
    NV_ID = "";
    startDate = "";
    endDate = "";
    reason = "";
    status = "Cho duyet";
    managerNote = "";
}

LeaveRequest::LeaveRequest(string requestID, string NV_ID, string startDate, string endDate, string reason, string status, string managerNote){
    this->requestID = requestID;
    this->NV_ID = NV_ID;
    this->startDate = startDate;
    this->endDate = endDate;
    this->reason = reason;
    this->status = status;
    this->managerNote = managerNote;
}

string LeaveRequest::getRequestID() const{
    return requestID;
}

string LeaveRequest::getNV_ID() const{
    return NV_ID;
}

string LeaveRequest::getStartDate() const{
    return startDate;
}

string LeaveRequest::getEndDate() const{
    return endDate;
}

string LeaveRequest::getReason() const{
    return reason;
}

string LeaveRequest::getStatus() const{
    return status;
}

string LeaveRequest::getManagerNote() const{
    return managerNote;
}

void LeaveRequest::setRequestID(string requestID){
    this->requestID = requestID;
}

void LeaveRequest::setNV_ID(string NV_ID){
    this->NV_ID = NV_ID;
}

void LeaveRequest::setStartDate(string startDate){
    this->startDate = startDate;
}

void LeaveRequest::setEndDate(string endDate){
    this->endDate = endDate;
}

void LeaveRequest::setReason(string reason){
    this->reason = reason;
}

void LeaveRequest::setStatus(string status){
    this->status = status;
}

void LeaveRequest::setManagerNote(string managerNote){
    this->managerNote = managerNote;
}