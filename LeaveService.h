#ifndef LEAVE_SERVICE_H
#define LEAVE_SERVICE_H

#include "../Repositories/LeaveRequestRepository.h"
#include "EmployeeService.h"

#include <vector>
#include <string>
using namespace std;

class LeaveService{
private:
    LeaveRequestRepository leaveRequestRepository;
    EmployeeService employeeService;

    string generateRequestID();

    bool isValidDate(const string& date);
    bool parseDate(const string& date, int& day, int& month, int& year);
    bool isDateBefore(const string& date1, const string& date2);
    bool isDateOverlap(const string& start1, const string& end1, const string& start2, const string& end2);

    string getCurrentDate();

public:
    LeaveService();

    // Lấy toàn bộ đơn
    vector<LeaveRequest> getAllRequests();

    // Lấy đơn của một nhân viên
    vector<LeaveRequest> getRequestsByEmployee(string NV_ID);

    // Lọc đơn theo trạng thái
    vector<LeaveRequest> getRequestsByStatus(string status);

    // Tìm đơn theo mã
    bool findByID(string requestID, LeaveRequest& request);

    // Nhân viên tạo đơn
    bool createRequest(string NV_ID, string startDate, string endDate, string reason, string& message);

    // Nhân viên hủy đơn đang chờ duyệt
    bool cancelRequest(string requestID, string NV_ID, string& message);

    // Quản lý duyệt đơn
    bool approveRequest(string requestID, string role, string managerNote, string& message);

    // Quản lý từ chối đơn
    bool rejectRequest(string requestID, string role, string managerNote, string& message);
};

#endif