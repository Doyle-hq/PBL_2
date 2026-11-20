#ifndef MENU_H
#define MENU_H

#include "raylib.h"

#include "../Services/AccountService.h"
#include "../Services/EmployeeService.h"
#include "../Services/DepartmentService.h"
#include "../Services/PositionService.h"
#include "../Services/ProductService.h"
#include "../Services/ProductionService.h"
#include "../Services/SalaryService.h"
#include "../Services/LeaveService.h"

#include "../Models/Account.h"
#include "../Models/Employee.h"
#include "../Models/Department.h"
#include "../Models/Position.h"
#include "../Models/Product.h"
#include "../Models/ProductionRecord.h"
#include "../Models/Salary.h"
#include "../Models/LeaveRequest.h"

#include <string>
#include <vector>

using namespace std;

enum class MenuScreen
{
    LOGIN,
    DASHBOARD,
    EMPLOYEES,
    EMPLOYEE_DETAIL,
    DEPARTMENTS,
    POSITIONS,
    PRODUCTS,
    PRODUCTION,
    SALARIES,
    LEAVE_MANAGEMENT,
    STATISTICS,
    MY_PROFILE,
    MY_SALARY,
    MY_LEAVE,
    CHANGE_PASSWORD,
};

class Menu{
private:
    // SERVICES
    AccountService accountService;
    EmployeeService employeeService;
    DepartmentService departmentService;
    PositionService positionService;
    ProductService productService;
    ProductionService productionService;
    SalaryService salaryService;
    LeaveService leaveService;

    // LOGIN AND SESSION
    Account currentAccount;
    bool isLoggedIn;

    MenuScreen currentScreen;
    MenuScreen previousScreen;

    // WINDOW AND FONT
    Font UI_FONT;
    bool fontLoaded;

    int screenWidth;
    int screenHeight;

    // SIDEBAR
    int sidebarWidth;
    int headerHeight;

    // INPUT
    string inputText[10];
    bool inputActive[10];

    // UI STATE
    string notification;
    float notificationTimer;

    string selectedID;
    string searchText;

    int selectedMonth;
    int selectedYear;

    int selectedTab;
    int scrollOffset;

    // EMPLOYEE FORM
    string employeeIDInput;
    string employeeNameInput;
    string employeeDOBInput;
    string employeeGenderInput;
    string employeePhoneInput;
    string employeeAddressInput;
    string employeeDepartmentInput;
    string employeePositionInput;

    // DEPARTMENT FORM
    string departmentNameInput;

    // POSITION FORM
    string positionDepartmentInput;
    string positionNameInput;
    string positionTypeInput;
    string positionCoefficientInput;

    // PRODUCT FORM
    string productIDInput;
    string productNameInput;
    string productPriceInput;

    // PRODUCTION FORM
    string productionEmployeeInput;
    string productionProductInput;
    string productionQuantityInput;
    string productionMonthInput;
    string productionYearInput;

    // SALARY FORM
    string salaryEmployeeInput;
    string salaryBaseInput;
    string salaryAllowanceInput;
    string salaryBonusInput;
    string salaryDeductionInput;
    string salaryMonthInput;
    string salaryYearInput;

    // LEAVE FORM
    string leaveStartDateInput;
    string leaveEndDateInput;
    string leaveReasonInput;
    string leaveManagerNoteInput;

    // PASSWORD FORM
    string oldPasswordInput;
    string newPasswordInput;
    string confirmPasswordInput;

    // CONFIRMATION DIALOG
    bool showConfirmDialog;
    string confirmTitle;
    string confirmMessage;
    int confirmAction;

    // INITIALIZATION
    void initialize();
    void loadVietnameseFont();
    void unloadVietnameseFont();

    // DRAW HELPERS
    void drawText(const string& text, int x, int y, int fontSize, Color color);
    void drawCenteredText(const string& text, int centerX, int y, int fontSize, Color color);
    bool drawButton(Rectangle bounds, const string& text, Color normalColor, Color hoverColor);
    bool drawInputBox(Rectangle bounds, string& value, const string& placeholder, bool password = false);
    void drawPanel(Rectangle bounds, Color color);
    void drawHeader(const string& title);
    void drawSidebar();
    void drawNotification();
    void drawConfirmDialog();

    // SCREEN DRAWING
    void drawLoginScreen();
    void drawDashboard();
    void drawEmployeeManagement();
    void drawEmployeeDetail();
    void drawDepartmentManagement();
    void drawPositionManagement();
    void drawProductManagement();
    void drawProductionManagement();
    void drawSalaryManagement();
    void drawLeaveManagement();
    void drawStatistics();
    void drawMyProfile();
    void drawMySalary();
    void drawMySalaryDetail();
    void drawMyLeave();
    void drawChangePassword();

    // SCREEN NAVIGATION
    void navigateTo(MenuScreen screen);
    void handleSidebarNavigation();
    bool isManager() const;
    bool isEmployee() const;

    // LOGIN
    void handleLogin();
    void handleLogout();

    // EMPLOYEE OPERATIONS
    void handleAddEmployee();
    void handleUpdateEmployee();
    void handleDeleteEmployee();
    void handleResignation();
    void handleUpdateMyProfile();

    // DEPARTMENT OPERATIONS
    void handleAddDepartment();
    void handleUpdateDepartment();
    void handleDeleteDepartment();

    // POSITION OPERATIONS
    void handleAddPosition();
    void handleUpdatePosition();
    void handleDeletePosition();

    // PRODUCT OPERATIONS
    void handleAddProduct();
    void handleUpdateProduct();
    void handleActivateProduct();
    void handleDeactivateProduct();

    // PRODUCTION OPERATIONS
    void handleAddProduction();
    void handleUpdateProduction();
    void handleDeleteProduction();

    // SALARY OPERATIONS
    void handleCreateSalary();

    // LEAVE OPERATIONS
    void handleCreateLeaveRequest();
    void handleCancelLeaveRequest();
    void handleApproveLeaveRequest();
    void handleRejectLeaveRequest();

    // EMPLOYEE PERSONAL OPERATIONS
    void handleUpdateMyContactInfo();
    void handleChangePassword();

    // CONFIRMATION
    void requestConfirmation(const string& title, const string& message, int action);
    void executeConfirmedAction();

    // VALIDATION AND FORMAT
    bool isNumber(const string& value);
    double toDouble(const string& value);
    int toInt(const string& value);
    string formatMoney(double value);
    string getDepartmentName(string PHG_ID);
    void showNotification(const string& message);

    string managerNoteInput;
    string statMonthInput;
    string statYearInput;
    void handleApproveLeave();
    void handleRejectLeave();

    string profilePhoneInput;
    string profileAddressInput;
    bool isEditingProfile;

    string selectedSalaryID;
    string selectedLeaveRequestID;

    string currentPasswordInput;

public:
    Menu();
    ~Menu();
    void update();
    void draw();
    void run();
};

#endif