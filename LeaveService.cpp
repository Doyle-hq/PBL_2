#include "LeaveService.h"

#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

// Constructor
LeaveService::LeaveService() : leaveRequestRepository("Data/LeaveRequest.txt"){}

// Chuyển ngày dd/MM/yyyy thành các số ngày, tháng, năm
bool LeaveService::parseDate(const string& date, int& day, int& month, int& year){
    if(date.length() != 10 || date[2] != '/' || date[5] != '/'){
        return false;
    }

    for(int i = 0; i < 10; i++){
        if(i == 2 || i == 5) continue;

        if(!isdigit(static_cast<unsigned char>(date[i]))){
            return false;
        }
    }

    day = stoi(date.substr(0, 2));
    month = stoi(date.substr(3, 2));
    year = stoi(date.substr(6, 4));

    if(year < 2000 || month < 1 || month > 12){
        return false;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    // Kiểm tra năm nhuận
    bool leapYear = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

    if(leapYear){
        daysInMonth[1] = 29;
    }

    if(day < 1 || day > daysInMonth[month - 1]){
        return false;
    }

    return true;
}

bool LeaveService::isValidDate(const string& date){
    int day, month, year;
    return parseDate(date, day, month, year);
}

// So sánh hai ngày
bool LeaveService::isDateBefore(const string& date1, const string& date2){
    int d1, m1, y1;
    int d2, m2, y2;

    if(!parseDate(date1, d1, m1, y1) || !parseDate(date2, d2, m2, y2)){
        return false;
    }

    if(y1 != y2) return y1 < y2;
    if(m1 != m2) return m1 < m2;

    return d1 < d2;
}

// Kiểm tra hai khoảng thời gian có giao nhau không
bool LeaveService::isDateOverlap(const string& start1, const string& end1, const string& start2, const string& end2){
    return !isDateBefore(end1, start2) && !isDateBefore(end2, start1);
}

// Lấy ngày hiện tại
string LeaveService::getCurrentDate(){
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    if(localTime == nullptr){
        return "";
    }

    stringstream ss;

    ss << setfill('0')
       << setw(2) << localTime->tm_mday << "/"
       << setw(2) << localTime->tm_mon + 1 << "/"
       << setw(4) << localTime->tm_year + 1900;

    return ss.str();
}

// Tạo mã đơn NP000001, NP000002...
string LeaveService::generateRequestID(){
    vector<LeaveRequest> requests = leaveRequestRepository.getAll();

    int maxID = 0;

    for(const LeaveRequest& request : requests){
        string id = request.getRequestID();

        if(id.length() != 8 || id.substr(0, 2) != "NP"){
            continue;
        }

        try{
            int number = stoi(id.substr(2));

            if(number > maxID){
                maxID = number;
            }
        }
        catch(...){
            continue;
        }
    }

    stringstream ss;

    ss << "NP" << setfill('0') << setw(6) << maxID + 1;

    return ss.str();
}

// Lấy toàn bộ đơn
vector<LeaveRequest> LeaveService::getAllRequests(){
    return leaveRequestRepository.getAll();
}

// Lấy đơn theo nhân viên
vector<LeaveRequest> LeaveService::getRequestsByEmployee(string NV_ID){
    vector<LeaveRequest> result;

    vector<LeaveRequest> requests = leaveRequestRepository.getAll();

    for(const LeaveRequest& request : requests){
        if(request.getNV_ID() == NV_ID){
            result.push_back(request);
        }
    }

    return result;
}

// Lọc theo trạng thái
vector<LeaveRequest> LeaveService::getRequestsByStatus(string status){
    vector<LeaveRequest> result;

    vector<LeaveRequest> requests = leaveRequestRepository.getAll();

    for(const LeaveRequest& request : requests){
        if(request.getStatus() == status){
            result.push_back(request);
        }
    }

    return result;
}

// Tìm đơn theo mã
bool LeaveService::findByID(string requestID, LeaveRequest& request){
    return leaveRequestRepository.findByID(requestID, request);
}

// Nhân viên tạo đơn
bool LeaveService::createRequest(string NV_ID, string startDate, string endDate, string reason, string& message){
    // Kiểm tra nhân viên
    Employee employee;

    if(!employeeService.findByID(NV_ID, employee)){
        message = "Khong tim thay nhan vien!";
        return false;
    }

    if(employee.getStatus() != "Dang lam"){
        message = "Nhan vien khong con lam viec!";
        return false;
    }

    // Kiểm tra dữ liệu
    if(!isValidDate(startDate) || !isValidDate(endDate)){
        message = "Ngay nghi khong hop le! Dinh dang: dd/MM/yyyy";
        return false;
    }

    if(isDateBefore(endDate, startDate)){
        message = "Ngay ket thuc phai sau ngay bat dau!";
        return false;
    }

    string today = getCurrentDate();

    if(today.empty()){
        message = "Khong xac dinh duoc ngay hien tai!";
        return false;
    }

    if(isDateBefore(startDate, today)){
        message = "Khong the xin nghi trong qua khu!";
        return false;
    }

    if(reason.empty()){
        message = "Ly do nghi khong duoc de trong!";
        return false;
    }

    if(reason.find(',') != string::npos || reason.find('\n') != string::npos || reason.find('\r') != string::npos){
        message = "Ly do khong duoc chua dau phay hoac xuong dong!";
        return false;
    }

    // Kiểm tra trùng thời gian
    vector<LeaveRequest> requests = leaveRequestRepository.getAll();

    for(const LeaveRequest& current : requests){
        if(current.getNV_ID() != NV_ID){
            continue;
        }

        string status = current.getStatus();

        if(status != "Cho duyet" && status != "Da duyet"){
            continue;
        }

        if(isDateOverlap(startDate, endDate, current.getStartDate(), current.getEndDate())){
            message = "Khoang thoi gian nay trung voi don nghi khac!";
            return false;
        }
    }

    // Tạo đơn mới
    string requestID = generateRequestID();

    LeaveRequest request(requestID, NV_ID, startDate, endDate, reason, "Cho duyet", "");

    if(!leaveRequestRepository.add(request)){
        message = "Khong the luu don xin nghi!";
        return false;
    }

    message = "Tao don xin nghi thanh cong! Ma don: " + requestID;
    return true;
}

// Nhân viên hủy đơn
bool LeaveService::cancelRequest(string requestID, string NV_ID, string& message){
    LeaveRequest request;

    if(!leaveRequestRepository.findByID(requestID, request)){
        message = "Khong tim thay don xin nghi!";
        return false;
    }

    if(request.getNV_ID() != NV_ID){
        message = "Ban khong co quyen thao tac don nay!";
        return false;
    }

    if(request.getStatus() != "Cho duyet"){
        message = "Chi co the huy don dang cho duyet!";
        return false;
    }

    // Giữ lại lịch sử, không xóa khỏi file
    request.setStatus("Da huy");

    if(!leaveRequestRepository.update(request)){
        message = "Khong the cap nhat don!";
        return false;
    }

    message = "Huy don xin nghi thanh cong!";
    return true;
}

// Quản lý duyệt đơn
bool LeaveService::approveRequest(string requestID, string role, string managerNote, string& message){
    if(role != "manager"){
        message = "Ban khong co quyen duyet don!";
        return false;
    }

    LeaveRequest request;

    if(!leaveRequestRepository.findByID(requestID, request)){
        message = "Khong tim thay don xin nghi!";
        return false;
    }

    if(request.getStatus() != "Cho duyet"){
        message = "Don nay khong con o trang thai cho duyet!";
        return false;
    }

    if(managerNote.find(',') != string::npos || managerNote.find('\n') != string::npos || managerNote.find('\r') != string::npos){
        message = "Ghi chu khong duoc chua dau phay hoac xuong dong!";
        return false;
    }

    request.setStatus("Da duyet");
    request.setManagerNote(managerNote);

    if(!leaveRequestRepository.update(request)){
        message = "Khong the cap nhat don!";
        return false;
    }

    message = "Da duyet don xin nghi!";
    return true;
}

// Quản lý từ chối đơn
bool LeaveService::rejectRequest(string requestID, string role, string managerNote, string& message){
    if(role != "manager"){
        message = "Ban khong co quyen tu choi don!";
        return false;
    }

    LeaveRequest request;

    if(!leaveRequestRepository.findByID(requestID, request)){
        message = "Khong tim thay don xin nghi!";
        return false;
    }

    if(request.getStatus() != "Cho duyet"){
        message = "Don nay khong con o trang thai cho duyet!";
        return false;
    }

    if(managerNote.empty()){
        message = "Can nhap ly do tu choi!";
        return false;
    }

    if(managerNote.find(',') != string::npos || managerNote.find('\n') != string::npos || managerNote.find('\r') != string::npos){
        message = "Ghi chu khong duoc chua dau phay hoac xuong dong!";
        return false;
    }

    request.setStatus("Tu choi");
    request.setManagerNote(managerNote);

    if(!leaveRequestRepository.update(request)){
        message = "Khong the cap nhat don!";
        return false;
    }

    message = "Da tu choi don xin nghi!";
    return true;
}