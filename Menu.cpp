#include "Menu.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cctype>

using namespace std;

// COLORS
static const Color COLOR_BACKGROUND = {245, 247, 251, 255};
static const Color COLOR_SIDEBAR = {25, 43, 72, 255};
static const Color COLOR_PRIMARY = {48, 112, 190, 255};
static const Color COLOR_PRIMARY_HOVER = {34, 91, 161, 255};
static const Color COLOR_WHITE = {255, 255, 255, 255};
static const Color COLOR_TEXT = {45, 55, 72, 255};
static const Color COLOR_GRAY = {125, 135, 150, 255};
static const Color COLOR_BORDER = {220, 226, 235, 255};
static const Color COLOR_SUCCESS = {35, 155, 100, 255};
static const Color COLOR_DANGER = {210, 65, 65, 255};
static const Color COLOR_WARNING = {225, 155, 40, 255};
static const Color COLOR_ROW_HOVER = {225, 238, 250, 255};
static const Color COLOR_ROW_EVEN  = {245, 247, 250, 255};
static const Color COLOR_GRAY_HOVER = {100, 100, 100, 255};
// VIETNAMESE FONT CODEPOINTS
static vector<int> createVietnameseCodepoints(){
    vector<int> codepoints;

    // ASCII
    for(int i = 32; i <= 126; i++){
        codepoints.push_back(i);
    }

    // Latin mở rộng
    for(int i = 0x00C0; i <= 0x024F; i++){
        codepoints.push_back(i);
    }

    // Các ký tự tiếng Việt dựng sẵn
    for(int i = 0x1E00; i <= 0x1EFF; i++){
        codepoints.push_back(i);
    }

    // Dấu kết hợp
    for(int i = 0x0300; i <= 0x036F; i++){
        codepoints.push_back(i);
    }

    return codepoints;
}

// UTF-8 INPUT
// ENCODE UTF-8
static string encodeUTF8(int codepoint){
    string result;

    if(codepoint < 0 || codepoint > 0x10FFFF || (codepoint >= 0xD800 && codepoint <= 0xDFFF)){
        return result;
    }

    if(codepoint <= 0x7F){
        result += static_cast<char>(codepoint);
    }
    else if(codepoint <= 0x7FF){
        result += static_cast<char>(0xC0 | (codepoint >> 6));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else if(codepoint <= 0xFFFF){
        result += static_cast<char>(0xE0 | (codepoint >> 12));
        result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else{
        result += static_cast<char>(0xF0 | (codepoint >> 18));
        result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }

    return result;
}

static void removeLastUTF8Character(string& value){
    if(value.empty()) return;

    int i = static_cast<int>(value.size()) - 1;

    while(i > 0 && (static_cast<unsigned char>(value[i]) & 0xC0) == 0x80){
        i--;
    }

    value.erase(i);
}

// CONSTRUCTOR
Menu::Menu(){
    initialize();
}

Menu::~Menu(){
    unloadVietnameseFont();
}

// INITIALIZATION
void Menu::initialize(){
    isLoggedIn = false;

    currentScreen = MenuScreen::LOGIN;
    previousScreen = MenuScreen::LOGIN;

    UI_FONT = GetFontDefault();
    fontLoaded = false;

    screenWidth = 1280;
    screenHeight = 720;

    sidebarWidth = 245;
    headerHeight = 75;

    selectedMonth = 1;
    selectedYear = 2026;

    selectedTab = 0;
    scrollOffset = 0;

    notification = "";
    notificationTimer = 0;

    selectedID = "";
    searchText = "";

    showConfirmDialog = false;
    confirmTitle = "";
    confirmMessage = "";
    confirmAction = -1;

    for(int i = 0; i < 10; i++){
        inputText[i] = "";
        inputActive[i] = false;
    }

    employeeIDInput = "";
    employeeNameInput = "";
    employeeDOBInput = "";
    employeeGenderInput = "Nam";
    employeePhoneInput = "";
    employeeAddressInput = "";
    employeeDepartmentInput = "";
    employeePositionInput = "";

    departmentNameInput = "";

    positionDepartmentInput = "";
    positionNameInput = "";
    positionTypeInput = "Van phong";
    positionCoefficientInput = "";

    productIDInput = "";
    productNameInput = "";
    productPriceInput = "";

    productionEmployeeInput = "";
    productionProductInput = "";
    productionQuantityInput = "";
    productionMonthInput = "";
    productionYearInput = "";

    salaryEmployeeInput = "";
    salaryBaseInput = "";
    salaryAllowanceInput = "";
    salaryBonusInput = "";
    salaryDeductionInput = "";
    salaryMonthInput = "";
    salaryYearInput = "";

    leaveStartDateInput = "";
    leaveEndDateInput = "";
    leaveReasonInput = "";
    leaveManagerNoteInput = "";

    oldPasswordInput = "";
    newPasswordInput = "";
    confirmPasswordInput = "";

    managerNoteInput = "";

    statMonthInput = "";
    statYearInput = "";

    profilePhoneInput = "";
    profileAddressInput = "";
    isEditingProfile = false;

    selectedSalaryID = "";
    leaveStartDateInput = "";
    leaveEndDateInput = "";
    leaveReasonInput = "";
    selectedLeaveRequestID = "";

    currentPasswordInput = "";
    newPasswordInput = "";
    confirmPasswordInput = "";
}

// LOAD VIETNAMESE FONT
void Menu::loadVietnameseFont(){
    if(fontLoaded) return;

    vector<int> codepoints = createVietnameseCodepoints();

    UI_FONT = LoadFontEx("C:/Windows/Fonts/arial.ttf", 64, codepoints.data(), static_cast<int>(codepoints.size()));

    if(UI_FONT.texture.id == 0){
        UI_FONT = GetFontDefault();
        fontLoaded = false;
        return;
    }

    SetTextureFilter(UI_FONT.texture, TEXTURE_FILTER_BILINEAR);
    fontLoaded = true;
}

// UNLOAD FONT
void Menu::unloadVietnameseFont(){
    if(fontLoaded){
        UnloadFont(UI_FONT);
        fontLoaded = false;
    }
}

// MAIN LOOP
void Menu::run(){
    loadVietnameseFont();

    while(!WindowShouldClose()){
        update();

        BeginDrawing();

        ClearBackground(COLOR_BACKGROUND);

        draw();

        EndDrawing();
    }

    unloadVietnameseFont();
}

// UPDATE
void Menu::update(){
    if(notificationTimer > 0){
        notificationTimer -= GetFrameTime();
        if(notificationTimer <= 0){
            notification = "";
        }
    }

    if(currentScreen != MenuScreen::LOGIN && !isLoggedIn){
        currentScreen = MenuScreen::LOGIN;
    }

    if(currentScreen == MenuScreen::LOGIN && IsKeyPressed(KEY_ENTER)){
        handleLogin();
    }
}

// DRAW TEXT
void Menu::drawText(const string& text, int x, int y, int fontSize, Color color){
    float scaledFontSize = fontSize * 1.25f;
    DrawTextEx(UI_FONT, text.c_str(), Vector2{static_cast<float>(x), static_cast<float>(y)}, scaledFontSize, 1.0f, color);
}

// DRAW CENTERED TEXT
void Menu::drawCenteredText(const string& text, int centerX, int y, int fontSize, Color color){
    float scaledFontSize = fontSize * 1.25f;
    Vector2 size = MeasureTextEx(UI_FONT, text.c_str(), scaledFontSize, 1.0f);

    int x = centerX - static_cast<int>(size.x / 2);
    drawText(text, x, y, fontSize, color);
}

// DRAW PANEL
void Menu::drawPanel(Rectangle bounds, Color color){
    DrawRectangleRounded(bounds, 0.08f, 8, color);

    DrawRectangleRoundedLinesEx(bounds, 0.08f, 8, 1, COLOR_BORDER);
}

// DRAW BUTTON
bool Menu::drawButton(Rectangle bounds, const string& text, Color color, Color hoverColor){
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, bounds);

    DrawRectangleRounded(bounds, 0.2f, 8, hovered ? hoverColor : color);

    float scaledFontSize = 18 * 1.25f;
    Vector2 textSize = MeasureTextEx(UI_FONT, text.c_str(), scaledFontSize, 1.0f);

    float textX = bounds.x + (bounds.width - textSize.x) / 2.0f;
    float textY = bounds.y + (bounds.height - textSize.y) / 2.0f;

    DrawTextEx(UI_FONT, text.c_str(), Vector2{textX, textY}, scaledFontSize, 1.0f, COLOR_WHITE);

    return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// DRAW INPUT BOX WITH CURSOR
bool Menu::drawInputBox(Rectangle bounds, string& value, const string& placeholder, bool password){
    static int activeInput = -1;
    static float cursorTimer = 0.0f;

    int inputID = static_cast<int>(bounds.x * 1000 + bounds.y);
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, bounds);

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hovered){
        activeInput = inputID;
        cursorTimer = 0.0f;
    }

    bool active = (activeInput == inputID);

    if(active){
        cursorTimer += GetFrameTime();

        int key = GetCharPressed();
        while(key > 0){
            if(key >= 32 && key <= 0x10FFFF){
                value += encodeUTF8(key);
            }
            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE)){
            removeLastUTF8Character(value);
        }
    }

    DrawRectangleRounded(bounds, 0.12f, 8, COLOR_WHITE);
    DrawRectangleRoundedLinesEx(bounds, 0.12f, 8, 1.5f, active ? COLOR_PRIMARY : COLOR_BORDER);

    string displayValue = password ? string(value.length(), '*') : value;

    // Dong bo kich thuoc voi drawText() (x1.25)
    const float baseFontSize = 20.0f;
    const float actualFontSize = baseFontSize * 1.25f;

    Vector2 textSize = MeasureTextEx(UI_FONT, displayValue.empty() ? placeholder.c_str() : displayValue.c_str(), actualFontSize, 1.0f);
    float textY = bounds.y + (bounds.height - textSize.y) / 2.0f;

    if(displayValue.empty()){
        drawText(placeholder, static_cast<int>(bounds.x + 16), static_cast<int>(textY), static_cast<int>(baseFontSize), COLOR_GRAY);
    }
    else{
        drawText(displayValue, static_cast<int>(bounds.x + 16), static_cast<int>(textY), static_cast<int>(baseFontSize), COLOR_TEXT);
    }

    // Con tro dung theo chieu rong chu thuc te
    if(active && static_cast<int>(cursorTimer * 2) % 2 == 0){
        float textWidth = displayValue.empty() ? 0.0f : textSize.x;
        int cursorX = static_cast<int>(bounds.x + 16 + textWidth);

        DrawLine(cursorX, static_cast<int>(textY), cursorX, static_cast<int>(textY + textSize.y), COLOR_PRIMARY);
    }

    return active;
}

// HEADER
void Menu::drawHeader(const string& title){
    DrawRectangle(sidebarWidth, 0, GetScreenWidth() - sidebarWidth, headerHeight, COLOR_WHITE);

    DrawLine(sidebarWidth, headerHeight, GetScreenWidth(), headerHeight, COLOR_BORDER);

    drawText(title, sidebarWidth + 30, 23, 25, COLOR_TEXT);

    drawText(currentAccount.getNV_ID(), GetScreenWidth() - 190, 28, 15, COLOR_GRAY);
}

// SIDEBAR NAVIGATION

void Menu::navigateTo(MenuScreen screen){
    // Kiem tra quyen truy cap
    if(screen != MenuScreen::LOGIN){
        if(!isLoggedIn){
            screen = MenuScreen::LOGIN;
        }
        else if(isEmployee()){
            switch(screen){
                case MenuScreen::DASHBOARD:
                case MenuScreen::MY_PROFILE:
                case MenuScreen::MY_SALARY:
                case MenuScreen::MY_LEAVE:
                case MenuScreen::CHANGE_PASSWORD:
                    break;

                default:
                    screen = MenuScreen::DASHBOARD;
                    break;
            }
        }
        else if(!isManager()){
            screen = MenuScreen::LOGIN;
        }
    }

    previousScreen = currentScreen;
    currentScreen = screen;

    searchText = "";
    selectedID = "";
    scrollOffset = 0;
    selectedTab = 0;

    // Khi mo lai man hinh thong ke, mac dinh ve thang hien tai
    if(screen == MenuScreen::STATISTICS){
        statMonthInput = "";
        statYearInput = "";
    }
}


// ROLE CHECK
bool Menu::isManager() const{
    return isLoggedIn && currentAccount.getRole() == "manager";
}

bool Menu::isEmployee() const{
    return isLoggedIn && currentAccount.getRole() == "employee";
}

// SIDEBAR
void Menu::drawSidebar(){
    DrawRectangle(0, 0, sidebarWidth, GetScreenHeight(), COLOR_SIDEBAR);

    drawText("PBL2", 28, 25, 30, COLOR_WHITE);

    drawText("EMPLOYEE MANAGEMENT", 28, 62, 10, Fade(COLOR_WHITE, 0.65f));

    DrawLine(20, 95, sidebarWidth - 20, 95, Fade(COLOR_WHITE, 0.2f));

    int y = 115;

    auto drawMenuItem = [&](const string& label, MenuScreen screen){
        Rectangle bounds = {12, static_cast<float>(y), static_cast<float>(sidebarWidth - 24), 42};

        bool active = currentScreen == screen;

        Vector2 mouse = GetMousePosition();

        bool hovered = CheckCollisionPointRec(mouse, bounds);

        if(active){
            DrawRectangleRounded(bounds, 0.15f, 8, COLOR_PRIMARY);
        }
        else if(hovered){
            DrawRectangleRounded(bounds, 0.15f, 8, Fade(COLOR_WHITE, 0.1f));
        }

        drawText(label, 28, y + 11, 16, COLOR_WHITE);

        if(hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            navigateTo(screen);
        }

        y += 48;
    };

    if(isManager()){
        drawText("TONG QUAN", 25, y, 11, Fade(COLOR_WHITE, 0.55f));

        y += 25;

        drawMenuItem("Trang chủ", MenuScreen::DASHBOARD);

        drawText("QUAN LY", 25, y + 3, 11, Fade(COLOR_WHITE, 0.55f));

        y += 28;

        drawMenuItem("Nhan vien", MenuScreen::EMPLOYEES);
        drawMenuItem("Phong ban", MenuScreen::DEPARTMENTS);
        drawMenuItem("Chuc vu", MenuScreen::POSITIONS);
        drawMenuItem("San pham", MenuScreen::PRODUCTS);
        drawMenuItem("San luong", MenuScreen::PRODUCTION);
        drawMenuItem("Bang luong", MenuScreen::SALARIES);
        drawMenuItem("Don xin nghi", MenuScreen::LEAVE_MANAGEMENT);
        drawMenuItem("Thong ke", MenuScreen::STATISTICS);
    }
    else if(isEmployee()){
        drawText("CA NHAN", 25, y, 11, Fade(COLOR_WHITE, 0.55f));

        y += 25;

        drawMenuItem("Dashboard", MenuScreen::DASHBOARD);
        drawMenuItem("Ho so ca nhan", MenuScreen::MY_PROFILE);
        drawMenuItem("Lich su luong", MenuScreen::MY_SALARY);
        drawMenuItem("Don xin nghi", MenuScreen::MY_LEAVE);
        drawMenuItem("Doi mat khau", MenuScreen::CHANGE_PASSWORD);
    }

    DrawLine(20, GetScreenHeight() - 65, sidebarWidth - 20, GetScreenHeight() - 65, Fade(COLOR_WHITE, 0.2f));

    if(drawButton(Rectangle{18, static_cast<float>(GetScreenHeight() - 52), static_cast<float>(sidebarWidth - 36), 38}, "Dang xuat", COLOR_DANGER, RED)){
        handleLogout();
    }
}

// NOTIFICATION
void Menu::showNotification(const string& message){
    notification = message;
    notificationTimer = 3.0f;
}

void Menu::drawNotification(){
    if(notification.empty()) return;

    int width = 450;
    int x = GetScreenWidth() - width - 25;
    int y = 20;

    Rectangle bounds = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 55};

    DrawRectangleRounded(bounds, 0.15f, 8, COLOR_PRIMARY);

    drawText(notification, x + 15, y + 18, 15, COLOR_WHITE);
}

// DRAW CONFIRMATION DIALOG
void Menu::requestConfirmation(const string& title, const string& message, int action){
    showConfirmDialog = true;
    confirmTitle = title;
    confirmMessage = message;
    confirmAction = action;
}

void Menu::drawConfirmDialog(){
    if(!showConfirmDialog) return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.45f));

    Rectangle dialog = {GetScreenWidth() / 2.0f - 230, GetScreenHeight() / 2.0f - 110, 460, 220};

    drawPanel(dialog, COLOR_WHITE);

    drawText(confirmTitle, static_cast<int>(dialog.x + 25), static_cast<int>(dialog.y + 25), 22, COLOR_TEXT);

    drawText(confirmMessage, static_cast<int>(dialog.x + 25), static_cast<int>(dialog.y + 80), 16, COLOR_GRAY);

    if(drawButton(Rectangle{dialog.x + 25, dialog.y + 150, 180, 42}, "Xac nhan", COLOR_DANGER, RED)){
        showConfirmDialog = false;
        executeConfirmedAction();
    }

    if(drawButton(Rectangle{dialog.x + 255, dialog.y + 150, 180, 42}, "Huy", COLOR_GRAY, DARKGRAY)){
        showConfirmDialog = false;
        confirmAction = -1;
    }
}

// DRAW MAIN
void Menu::draw(){
    if(currentScreen == MenuScreen::LOGIN){
        drawLoginScreen();
        drawNotification();
        return;
    }

    drawSidebar();

    switch(currentScreen){
        case MenuScreen::DASHBOARD:
            drawDashboard();
            break;

        case MenuScreen::EMPLOYEES:
            drawEmployeeManagement();
            break;

        case MenuScreen::EMPLOYEE_DETAIL:
            drawEmployeeDetail();
            break;

        case MenuScreen::DEPARTMENTS:
            drawDepartmentManagement();
            break;

        case MenuScreen::POSITIONS:
            drawPositionManagement();
            break;

        case MenuScreen::PRODUCTS:
            drawProductManagement();
            break;

        case MenuScreen::PRODUCTION:
            drawProductionManagement();
            break;

        case MenuScreen::SALARIES:
            drawSalaryManagement();
            break;

        case MenuScreen::LEAVE_MANAGEMENT:
            drawLeaveManagement();
            break;

        case MenuScreen::STATISTICS:
            drawStatistics();
            break;

        case MenuScreen::MY_PROFILE:
            drawMyProfile();
            break;

        case MenuScreen::MY_SALARY:
            if(selectedTab == 1)
                drawMySalaryDetail();
            else
                drawMySalary();
            break;

        case MenuScreen::MY_LEAVE:
            drawMyLeave();
            break;

        case MenuScreen::CHANGE_PASSWORD:
            drawChangePassword();
            break;

        default:
            break;
    }
}

// DRAW LOGIN SCREEN
void Menu::drawLoginScreen(){
    ClearBackground(COLOR_BACKGROUND);

    // Khung dang nhap lon hon
    Rectangle panel = {GetScreenWidth() / 2.0f - 280, GetScreenHeight() / 2.0f - 280, 560, 560};
    drawPanel(panel, COLOR_WHITE);

    // Tieu de
    drawCenteredText("PBL2", static_cast<int>(panel.x + panel.width / 2), static_cast<int>(panel.y + 55), 46, COLOR_PRIMARY);
    drawCenteredText("HỆ THỐNG QUẢN LÝ NHÂN VIÊN", static_cast<int>(panel.x + panel.width / 2), static_cast<int>(panel.y + 125), 20, COLOR_GRAY);

    // O tai khoan
    drawText("Mã nhân viên / Tài khoản", static_cast<int>(panel.x + 55), static_cast<int>(panel.y + 190), 18, COLOR_TEXT);
    drawInputBox(Rectangle{panel.x + 55, panel.y + 225, 450, 58}, inputText[0], "Nhập mã nhân viên hoặc tài khoản", false);

    // O mat khau
    drawText("Mật khẩu", static_cast<int>(panel.x + 55), static_cast<int>(panel.y + 325), 18, COLOR_TEXT);
    drawInputBox(Rectangle{panel.x + 55, panel.y + 360, 450, 58},inputText[1], "Nhập mật khẩu", false);

    // Nut dang nhap
    if(drawButton(Rectangle{panel.x + 55, panel.y + 455, 450, 58}, "ĐĂNG NHẬP", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        handleLogin();
    }
}

// LOGIN HANDLER
void Menu::handleLogin(){
    if(inputText[0].empty() || inputText[1].empty()){
        showNotification("Vui long nhap tai khoan va mat khau!");
        return;
    }

    string message;
    Account loginAccount;

    if(!accountService.login(inputText[0], inputText[1], loginAccount, message)){
        showNotification(message);
        inputText[1] = "";
        return;
    }

    currentAccount = loginAccount;
    isLoggedIn = true;

    inputText[0] = "";
    inputText[1] = "";

    selectedTab = 0;

    navigateTo(MenuScreen::DASHBOARD);

    showNotification("Dang nhap thanh cong!");
}

// LOGOUT
void Menu::handleLogout(){
    isLoggedIn = false;

    currentAccount = Account();

    for(int i = 0; i < 10; i++){
        inputText[i] = "";
    }

    navigateTo(MenuScreen::LOGIN);

    showNotification("Da dang xuat!");
}

// NUMBER HELPERS
bool Menu::isNumber(const string& value){
    if(value.empty()) return false;

    bool hasDot = false;

    for(char c : value){
        if(c == '.'){
            if(hasDot) return false;
            hasDot = true;
        }
        else if(!isdigit(static_cast<unsigned char>(c))){
            return false;
        }
    }

    return true;
}

double Menu::toDouble(const string& value){
    try{
        return stod(value);
    }
    catch(...){
        return 0;
    }
}

int Menu::toInt(const string& value){
    try{
        return stoi(value);
    }
    catch(...){
        return 0;
    }
}

string Menu::formatMoney(double value){
    stringstream ss;

    ss << fixed << setprecision(0) << value;

    string result = ss.str();

    int position = static_cast<int>(result.length()) - 3;

    while(position > 0){
        result.insert(position, ".");
        position -= 3;
    }

    return result + " VND";
}

// DEPARTMENT NAME
string Menu::getDepartmentName(string PHG_ID){
    vector<Department> departments = departmentService.getAllDepartments();

    for(const Department& department : departments){
        if(department.getPHG_ID() == PHG_ID){
            return department.getDepartmentName();
        }
    }

    return "Khong xac dinh";
}

// DASHBOARD
void Menu::drawDashboard(){
    drawHeader("Dashboard");

    int x = sidebarWidth + 30;
    int y = headerHeight + 30;

    drawText(isManager() ? "Tong quan he thong" : "Xin chao!", x, y, 25, COLOR_TEXT);

    y += 55;

    if(isManager()){
        vector<Employee> employees = employeeService.getAllEmployees();

        int totalEmployees = static_cast<int>(employees.size());
        int workingEmployees = 0;
        int resignedEmployees = 0;

        for(const Employee& employee : employees){
            if(employee.getStatus() == "Dang lam"){
                workingEmployees++;
            }
            else if(employee.getStatus() == "Da nghi viec"){
                resignedEmployees++;
            }
        }

        vector<Department> departments = departmentService.getAllDepartments();

        int screenWidth = GetScreenWidth();
        int contentWidth = screenWidth - x - 60;
        int gap = 20;
        int cardWidth = (contentWidth - gap * 3) / 4;
        int cardHeight = 145;

        struct DashboardCard{
            string title;
            string value;
            Color color;
        };

        vector<DashboardCard> cards = {
            {"Tong nhan vien", to_string(totalEmployees), COLOR_PRIMARY},
            {"Dang lam viec", to_string(workingEmployees), COLOR_SUCCESS},
            {"Da nghi viec", to_string(resignedEmployees), COLOR_DANGER},
            {"Phong ban", to_string(departments.size()), COLOR_WARNING}
        };

        for(int i = 0; i < static_cast<int>(cards.size()); i++){
            Rectangle card = {
                static_cast<float>(x + i * (cardWidth + gap)),
                static_cast<float>(y),
                static_cast<float>(cardWidth),
                static_cast<float>(cardHeight)
            };

            drawPanel(card, COLOR_WHITE);

            drawText(cards[i].title, static_cast<int>(card.x + 22), static_cast<int>(card.y + 25), 18, COLOR_GRAY);

            drawText(cards[i].value, static_cast<int>(card.x + 22), static_cast<int>(card.y + 70), 36, cards[i].color);
        }

        y += 180;

        drawText("Thong tin he thong", x, y, 20, COLOR_TEXT);

        y += 45;

        Rectangle infoPanel = {
            static_cast<float>(x),static_cast<float>(y),static_cast<float>(contentWidth),260
        };

        drawPanel(infoPanel, COLOR_WHITE);

        drawText("Chuc nang quan ly:", x + 25, y + 25, 20, COLOR_TEXT);

        int leftX = x + 40;
        int rightX = x + contentWidth / 2;

        drawText("- Quan ly ho so nhan vien", leftX, y + 85, 18, COLOR_GRAY);
        drawText("- Quan ly phong ban va chuc vu", leftX, y + 140, 18, COLOR_GRAY);
        drawText("- Quan ly san pham va san luong", rightX, y + 85, 18, COLOR_GRAY);
        drawText("- Quan ly bang luong va don xin nghi", rightX, y + 140, 18, COLOR_GRAY);
    }
    else if(isEmployee()){
        Employee employee;

        if(employeeService.findByID(currentAccount.getNV_ID(), employee)){
            drawText("Ma nhan vien: " + employee.getNV_ID(), x, y, 17, COLOR_TEXT);
            drawText("Phong ban: " + getDepartmentName(employee.getPHG_ID()), x, y + 35, 17, COLOR_TEXT);
            drawText("Chuc vu: " + employee.getPosition(), x, y + 70, 17, COLOR_TEXT);
            drawText("Trang thai: " + employee.getStatus(), x, y + 105, 17, COLOR_SUCCESS);
        }
        else{
            drawText("Khong tim thay thong tin nhan vien!", x, y, 17, COLOR_DANGER);
        }

        Rectangle panel = {
            static_cast<float>(x), static_cast<float>(y + 160), 800, 150
        };

        drawPanel(panel, COLOR_WHITE);
        drawText("Chao mung ban den voi he thong quan ly nhan vien.", x + 25, y + 190, 18, COLOR_TEXT);
        drawText("Ban co the xem ho so, lich su luong va don xin nghi.", x + 25, y + 230, 15, COLOR_GRAY);
    }
}

// EMPLOYEE MANAGEMENT
void Menu::drawEmployeeManagement(){
    drawHeader("Quan ly nhan vien");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int x = sidebarWidth + 30;
    int y = headerHeight + 25;
    int contentWidth = screenWidth - x - 30;

    auto resetEmployeeInputs = [&](){
        employeeNameInput = "";
        employeeDOBInput = "";
        employeeGenderInput = "Nam";
        employeePhoneInput = "";
        employeeAddressInput = "";
        employeeDepartmentInput = "";
        employeePositionInput = "";
    };

    // ==================== HEADER QUAN LY NHAN VIEN ====================
    drawText("Danh sach nhan vien", x, y + 8, 25, COLOR_TEXT);

    static int sortMode = 0;

    int buttonY = y - 5;
    int buttonWidth = 125;
    int buttonHeight = 48;
    int buttonGap = 8;

    int sortStartX = x + 250;

    // Nut sap xep theo ma
    if(drawButton(Rectangle{
        (float)sortStartX, (float)buttonY,
        (float)buttonWidth, (float)buttonHeight
    }, "Theo ma NV", sortMode == 0 ? COLOR_PRIMARY : COLOR_GRAY, COLOR_PRIMARY_HOVER)){
        sortMode = 0;
        scrollOffset = 0;
    }

    // Nut sap xep theo ten
    if(drawButton(Rectangle{
        (float)(sortStartX + buttonWidth + buttonGap), (float)buttonY,
        (float)buttonWidth, (float)buttonHeight
    }, "Theo ten", sortMode == 1 ? COLOR_PRIMARY : COLOR_GRAY, COLOR_PRIMARY_HOVER)){
        sortMode = 1;
        scrollOffset = 0;
    }

    // Nut sap xep theo phong ban
    if(drawButton(Rectangle{
        (float)(sortStartX + (buttonWidth + buttonGap) * 2), (float)buttonY,
        (float)buttonWidth, (float)buttonHeight
    }, "Theo phong ban", sortMode == 2 ? COLOR_PRIMARY : COLOR_GRAY, COLOR_PRIMARY_HOVER)){
        sortMode = 2;
        scrollOffset = 0;
    }

    // Nut them nhan vien
    if(drawButton(Rectangle{
        (float)(screenWidth - 230), (float)buttonY, 190, 48
    }, selectedTab == 1 ? "Quay lai" : "+ Them nhan vien",
    COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        selectedTab = (selectedTab == 1) ? 0 : 1;
        resetEmployeeInputs();
    }

    y += 65;

    // ==================== FORM THEM NHAN VIEN ====================
    if(selectedTab == 1){
        int fieldGap = 40;
        int fieldWidth = (contentWidth - 100 - fieldGap) / 2;
        int leftX = x + 30;
        int rightX = leftX + fieldWidth + fieldGap;

        drawPanel(Rectangle{static_cast<float>(x), static_cast<float>(y), static_cast<float>(contentWidth), 530}, COLOR_WHITE);

        // Ho ten va ngay sinh
        drawText("Ho ten", leftX, y + 20, 17, COLOR_TEXT);
        drawInputBox(Rectangle{static_cast<float>(leftX), static_cast<float>(y + 50), static_cast<float>(fieldWidth), 50}, employeeNameInput, "Nhap ho ten");

        drawText("Ngay sinh (dd/MM/yyyy)", rightX, y + 20, 17, COLOR_TEXT);
        drawInputBox(Rectangle{static_cast<float>(rightX), static_cast<float>(y + 50), static_cast<float>(fieldWidth), 50}, employeeDOBInput, "01/01/2000");

        // Gioi tinh va so dien thoai
        drawText("Gioi tinh (Nam/Nu)", leftX, y + 135, 17, COLOR_TEXT);
        drawInputBox(Rectangle{static_cast<float>(leftX), static_cast<float>(y + 165), static_cast<float>(fieldWidth), 50}, employeeGenderInput, "Nam");

        drawText("So dien thoai", rightX, y + 135, 17, COLOR_TEXT);
        drawInputBox(Rectangle{static_cast<float>(rightX), static_cast<float>(y + 165), static_cast<float>(fieldWidth), 50}, employeePhoneInput, "Nhap so dien thoai");

        // Dia chi
        drawText("Dia chi", leftX, y + 250, 17, COLOR_TEXT);
        drawInputBox(Rectangle{static_cast<float>(leftX), static_cast<float>(y + 280), static_cast<float>(contentWidth - 60), 50}, employeeAddressInput, "Nhap dia chi");

        // Phong ban va chuc vu
        drawText("Ma phong ban", leftX, y + 365, 17, COLOR_TEXT);
        drawInputBox(Rectangle{static_cast<float>(leftX), static_cast<float>(y + 395), static_cast<float>(fieldWidth), 50}, employeeDepartmentInput, "Vi du: 103");

        drawText("Chuc vu", rightX, y + 365, 17, COLOR_TEXT);
        drawInputBox(Rectangle{static_cast<float>(rightX), static_cast<float>(y + 395), static_cast<float>(fieldWidth), 50}, employeePositionInput, "Nhap chuc vu");

        // Nut luu va huy
        if(drawButton(Rectangle{static_cast<float>(leftX), static_cast<float>(y + 465), 200, 48}, "Luu nhan vien", COLOR_SUCCESS, Fade(COLOR_SUCCESS, 0.85f))){
            handleAddEmployee();
        }

        if(drawButton(Rectangle{static_cast<float>(leftX + 225), static_cast<float>(y + 465), 160, 48}, "Huy", COLOR_GRAY, COLOR_GRAY_HOVER)){
            selectedTab = 0;
            resetEmployeeInputs();
        }

        return;
    }

    // ==================== TIM KIEM VA SAP XEP ====================
    drawInputBox(Rectangle{static_cast<float>(x), static_cast<float>(y), 450, 50}, searchText, "Tim theo ma hoac ten");

    // ==================== LAY VA LOC DANH SACH ====================
    vector<Employee> employees;
    if(sortMode == 1) employees = employeeService.sortEmployeesByName();
    else if(sortMode == 2) employees = employeeService.sortEmployeesByDepartment();
    else employees = employeeService.sortEmployeesByID();

    vector<Employee> filtered;
    for(const Employee& employee : employees){
        bool matchID = employee.getNV_ID().find(searchText) != string::npos;
        bool matchName = employee.getFullName().find(searchText) != string::npos;

        if(searchText.empty() || matchID || matchName){
            filtered.push_back(employee);
        }
    }

    // ==================== KICH THUOC BANG & CUON CHUOT ====================
    int tableY = y + 120;
    int headerHeightTable = 52;
    int rowHeight = 54;
    int tableBottom = screenHeight - 85;
    int availableHeight = tableBottom - tableY - headerHeightTable;
    int visibleRows = max(1, availableHeight / rowHeight);
    int maxScroll = max(0, static_cast<int>(filtered.size()) - visibleRows);

    scrollOffset = max(0, min(scrollOffset, maxScroll));

    Rectangle tableBounds = {static_cast<float>(x), static_cast<float>(tableY), static_cast<float>(contentWidth), static_cast<float>(tableBottom - tableY)};

    if(CheckCollisionPointRec(GetMousePosition(), tableBounds)){
        float wheel = GetMouseWheelMove();
        if(wheel > 0) scrollOffset = max(0, scrollOffset - 3);
        else if(wheel < 0) scrollOffset = min(maxScroll, scrollOffset + 3);
    }

    // ==================== KHUNG & HEADER BANG ====================
    DrawRectangleRounded(tableBounds, 0.015f, 8, COLOR_WHITE);
    DrawRectangleRounded(Rectangle{static_cast<float>(x), static_cast<float>(tableY), static_cast<float>(contentWidth), static_cast<float>(headerHeightTable)}, 0.08f, 8, COLOR_PRIMARY);

    int colID = x + 20;
    int colName = x + 150;
    int colGender = x + static_cast<int>(contentWidth * 0.37f);
    int colDepartment = x + static_cast<int>(contentWidth * 0.47f);
    int colPosition = x + static_cast<int>(contentWidth * 0.69f);
    int colStatus = x + static_cast<int>(contentWidth * 0.87f);

    drawText("Ma NV", colID, tableY + 15, 16, COLOR_WHITE);
    drawText("Ho ten", colName, tableY + 15, 16, COLOR_WHITE);
    drawText("Gioi tinh", colGender, tableY + 15, 16, COLOR_WHITE);
    drawText("Phong ban", colDepartment, tableY + 15, 16, COLOR_WHITE);
    drawText("Chuc vu", colPosition, tableY + 15, 16, COLOR_WHITE);
    drawText("Trang thai", colStatus, tableY + 15, 16, COLOR_WHITE);

    // ==================== DANH SACH NHAN VIEN ====================
    int rowY = tableY + headerHeightTable;

    for(int i = scrollOffset; i < static_cast<int>(filtered.size()) && i < scrollOffset + visibleRows; i++){
        const Employee& employee = filtered[i];
        Rectangle row = {static_cast<float>(x), static_cast<float>(rowY), static_cast<float>(contentWidth), static_cast<float>(rowHeight)};

        DrawRectangleRec(row, i % 2 == 0 ? COLOR_WHITE : COLOR_BACKGROUND);

        drawText(employee.getNV_ID(), colID, rowY + 15, 15, COLOR_TEXT);
        drawText(employee.getFullName(), colName, rowY + 15, 15, COLOR_TEXT);
        drawText(employee.getGender(), colGender, rowY + 15, 15, COLOR_TEXT);
        drawText(getDepartmentName(employee.getPHG_ID()), colDepartment, rowY + 15, 15, COLOR_TEXT);
        drawText(employee.getPosition(), colPosition, rowY + 15, 15, COLOR_TEXT);

        Color statusColor = (employee.getStatus() == "Dang lam") ? COLOR_SUCCESS : COLOR_DANGER;
        drawText(employee.getStatus(), colStatus, rowY + 15, 15, statusColor);

        if(CheckCollisionPointRec(GetMousePosition(), row) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            string clickedID = employee.getNV_ID();
            navigateTo(MenuScreen::EMPLOYEE_DETAIL);
            selectedID = clickedID;
            return;
        }

        rowY += rowHeight;
    }

    if(filtered.empty()){
        drawText("Khong tim thay nhan vien.", x + 25, rowY + 20, 17, COLOR_GRAY);
    }

    // ==================== THONG TIN & NUT PHAN TRANG ====================
    int total = static_cast<int>(filtered.size());
    int firstRecord = (total == 0) ? 0 : scrollOffset + 1;
    int lastRecord = min(total, scrollOffset + visibleRows);

    drawText("Hien thi " + to_string(firstRecord) + " - " + to_string(lastRecord) + " / " + to_string(total) + " nhan vien", x + 10, screenHeight - 58, 15, COLOR_GRAY);

    if(drawButton(Rectangle{static_cast<float>(screenWidth - 245), static_cast<float>(screenHeight - 65), 90, 42}, "Truoc", COLOR_GRAY, COLOR_GRAY_HOVER)){
        scrollOffset = max(0, scrollOffset - visibleRows);
    }

    if(drawButton(Rectangle{static_cast<float>(screenWidth - 140), static_cast<float>(screenHeight - 65), 100, 42}, "Sau", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        scrollOffset = min(maxScroll, scrollOffset + visibleRows);
    }
}

// EMPLOYEE DETAIL
void Menu::drawEmployeeDetail(){
    drawHeader("Chi tiet nhan vien");

    int screenWidth = GetScreenWidth();
    int x = sidebarWidth + 30;
    int y = headerHeight + 25;
    int contentWidth = screenWidth - x - 30;

    Employee employee;

    if(!employeeService.findByID(selectedID, employee)){
        drawText("Khong tim thay nhan vien!", x, y, 20, COLOR_DANGER);

        if(drawButton(Rectangle{static_cast<float>(x), static_cast<float>(y + 65), 170, 48}, "Quay lai", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            navigateTo(MenuScreen::EMPLOYEES);
        }

        return;
    }

    drawText("Ho so nhan vien: " + employee.getFullName(), x, y, 26, COLOR_TEXT);
    y += 65;

    // Khung thong tin
    drawPanel(Rectangle{static_cast<float>(x), static_cast<float>(y), static_cast<float>(contentWidth), 410}, COLOR_WHITE);

    int leftX = x + 35;
    int rightX = x + contentWidth / 2 + 20;
    int textY = y + 35;
    int lineGap = 65;

    // Cot trai
    drawText("THONG TIN CA NHAN", leftX, textY, 20, COLOR_PRIMARY);
    drawText("Ma nhan vien: " + employee.getNV_ID(), leftX, textY + lineGap, 18, COLOR_TEXT);
    drawText("Ho ten: " + employee.getFullName(), leftX, textY + lineGap * 2, 18, COLOR_TEXT);
    drawText("Ngay sinh: " + employee.getDateOfBirth(), leftX, textY + lineGap * 3, 18, COLOR_TEXT);
    drawText("Gioi tinh: " + employee.getGender(), leftX, textY + lineGap * 4, 18, COLOR_TEXT);
    drawText("So dien thoai: " + employee.getPhone(), leftX, textY + lineGap * 5, 18, COLOR_TEXT);
    drawText("Dia chi: " + employee.getAddress(), leftX, textY + lineGap * 6, 18, COLOR_TEXT);

    // Cot phai
    drawText("THONG TIN CONG VIEC", rightX, textY, 20, COLOR_PRIMARY);
    drawText("Phong ban: " + getDepartmentName(employee.getPHG_ID()), rightX, textY + lineGap, 18, COLOR_TEXT);
    drawText("Ma phong: " + employee.getPHG_ID(), rightX, textY + lineGap * 2, 18, COLOR_TEXT);
    drawText("Chuc vu: " + employee.getPosition(), rightX, textY + lineGap * 3, 18, COLOR_TEXT);
    drawText("Ngay vao lam: " + employee.getNgayVaoLam(), rightX, textY + lineGap * 4, 18, COLOR_TEXT);

    Color statusColor = (employee.getStatus() == "Dang lam") ? COLOR_SUCCESS : COLOR_DANGER;
    drawText("Trang thai: " + employee.getStatus(), rightX, textY + lineGap * 5, 18, statusColor);

    // Cac nut thao tac
    int buttonY = y + 435;

    if(drawButton(Rectangle{static_cast<float>(x), static_cast<float>(buttonY), 170, 48}, "Quay lai", COLOR_GRAY, COLOR_GRAY_HOVER)){
        navigateTo(MenuScreen::EMPLOYEES);
        return;
    }

    if(employee.getStatus() == "Dang lam"){
        if(drawButton(Rectangle{static_cast<float>(x + 195), static_cast<float>(buttonY), 220, 48}, "Xac nhan nghi viec", COLOR_WARNING, ORANGE)){
            selectedID = employee.getNV_ID();
            requestConfirmation("Xac nhan nghi viec", "Ban co chac muon xac nhan nhan vien nay nghi viec?", 2);
        }

        if(drawButton(Rectangle{static_cast<float>(x + 440), static_cast<float>(buttonY), 190, 48}, "Xoa nhan vien", COLOR_DANGER, RED)){
            selectedID = employee.getNV_ID();
            requestConfirmation("Xoa nhan vien", "Thao tac nay se xoa ho so nhan vien!", 1);
        }
    }
}

// ADD EMPLOYEE
void Menu::handleAddEmployee(){
    Employee employee("", employeeNameInput, employeeDOBInput, employeeGenderInput, employeePhoneInput, employeeAddressInput, employeeDepartmentInput, "", "", employeePositionInput);

    string message;

    if(!employeeService.prepareEmployee(employee, message)){
        showNotification(message);
        return;
    }

    if(!employeeService.addEmployee(employee, message)){
        showNotification(message);
        return;
    }

    showNotification(message);

    employeeNameInput = "";
    employeeDOBInput = "";
    employeeGenderInput = "Nam";
    employeePhoneInput = "";
    employeeAddressInput = "";
    employeeDepartmentInput = "";
    employeePositionInput = "";

    selectedTab = 0;
}

// DELETE EMPLOYEE
void Menu::handleDeleteEmployee(){
    string message;

    if(employeeService.deleteEmployee(selectedID, message)){
        showNotification(message);
        selectedID = "";
        navigateTo(MenuScreen::EMPLOYEES);
    }
    else{
        showNotification(message);
    }
}

// CONFIRM RESIGNATION
void Menu::handleResignation(){
    string message;

    if(employeeService.confirmResignation(selectedID, message)){
        showNotification(message);
        selectedID = "";
        navigateTo(MenuScreen::EMPLOYEES);
    }
    else{
        showNotification(message);
    }
}

// CONFIRMED ACTIONS
void Menu::executeConfirmedAction()
{
    switch (confirmAction)
    {
        case 1:
            handleDeleteEmployee();
            break;

        case 2:
            handleResignation();
            break;

        case 3:
            handleDeleteDepartment();
            break;

        case 4:
            handleDeletePosition();
            break;

        case 5:
            handleDeactivateProduct();
            break;

        case 6:
            handleActivateProduct();
            break;

        case 7:
            handleDeleteProduction();
            break;

        case 8:
            handleApproveLeave();
            break;

        case 9:
            handleRejectLeave();
            break;

        case 10:
            handleUpdateMyProfile();
            break;

        default:
            break;
    }

    confirmAction = -1;
}

// DEPARTMENT MANAGEMENT
void Menu::drawDepartmentManagement(){
    drawHeader("QUAN LY PHONG BAN");

    drawPanel(Rectangle{280, 120, 1050, 700}, COLOR_WHITE);

    drawText("DANH SACH PHONG BAN", 310, 145, 24, COLOR_PRIMARY);

    // Nut them phong ban
    if(drawButton(Rectangle{1050, 135, 230, 45}, "THEM PHONG BAN", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        selectedTab = 1;
        inputText[0] = "";
        inputText[1] = "";
    }

    // O tim kiem
    drawInputBox(Rectangle{310, 205, 450, 45}, inputText[2], "Tim theo ma hoac ten", false);

    vector<Department> departments = departmentService.getAllDepartments();

    // Loc danh sach
    vector<Department> filtered;
    string keyword = inputText[2];

    for(const Department& department : departments){
        string id = department.getPHG_ID();
        string name = department.getDepartmentName();

        if(keyword.empty() || id.find(keyword) != string::npos || name.find(keyword) != string::npos){
            filtered.push_back(department);
        }
    }

    // Tieu de bang
    DrawRectangle(310, 275, 950, 45, COLOR_BORDER);

    drawText("MA PHONG", 340, 288, 20, COLOR_PRIMARY);
    drawText("TEN PHONG BAN", 650, 288, 20, COLOR_PRIMARY);

    int startY = 330;

    for(int i = 0; i < (int)filtered.size(); i++){
        int y = startY + i * 55;

        if(y > 770) break;

        DrawRectangle(310, y, 950, 50, i % 2 == 0 ? COLOR_WHITE : Fade(COLOR_BORDER, 0.5f));

        drawText(filtered[i].getPHG_ID(), 340, y + 15, 20, COLOR_TEXT);
        drawText(filtered[i].getDepartmentName(), 650, y + 15, 20, COLOR_TEXT);

        if(drawButton(Rectangle{1080, (float)y + 5, 75, 38}, "SUA", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            selectedTab = 2;
            selectedID = filtered[i].getPHG_ID();
            inputText[0] = filtered[i].getPHG_ID();
            inputText[1] = filtered[i].getDepartmentName();
        }

        if(drawButton(Rectangle{1170, (float)y + 5, 75, 38}, "XOA", COLOR_DANGER, RED)){
            selectedID = filtered[i].getPHG_ID();
            confirmAction = 3;
            showConfirmDialog = true;
        }
    }

    // Form them phong ban
    if(selectedTab == 1){
        drawPanel(Rectangle{430, 300, 600, 300}, COLOR_WHITE);

        drawText("THEM PHONG BAN", 620, 325, 24, COLOR_PRIMARY);

        drawInputBox(Rectangle{480, 390, 500, 45}, inputText[0], "Ten phong ban", false);

        if(drawButton(Rectangle{480, 480, 220, 45}, "LUU", COLOR_SUCCESS, DARKGREEN)){
            handleAddDepartment();
        }

        if(drawButton(Rectangle{740, 480, 220, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }

    // Form sua phong ban
    if(selectedTab == 2){
        drawPanel(Rectangle{430, 300, 600, 300}, COLOR_WHITE);

        drawText("CAP NHAT PHONG BAN", 590, 325, 24, COLOR_PRIMARY);

        drawText("Ma phong: " + inputText[0], 480, 390, 20, COLOR_GRAY);

        drawInputBox(Rectangle{480, 430, 500, 45}, inputText[1], "Ten phong ban moi", false);

        if(drawButton(Rectangle{480, 500, 220, 45}, "CAP NHAT", COLOR_SUCCESS, DARKGREEN)){
            handleUpdateDepartment();
        }

        if(drawButton(Rectangle{740, 500, 220, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }
}

// ADD DEPARTMENT
void Menu::handleAddDepartment(){
    string message;

    // Lưu ý: Đã xóa tham số màu sắc trong showNotification để khớp với định nghĩa trong Menu.h
    if(departmentService.addDepartment(inputText[0], message)){
        showNotification(message);
        selectedTab = 0;
        inputText[0] = "";
    }
    else{
        showNotification(message);
    }
}

// UPDATE DEPARTMENT
void Menu::handleUpdateDepartment(){
    string message;

    if(departmentService.updateDepartment(inputText[0], inputText[1], message)){
        showNotification(message);
        selectedTab = 0;
        inputText[0] = "";
        inputText[1] = "";
    }
    else{
        showNotification(message);
    }
}

// DELETE DEPARTMENT
void Menu::handleDeleteDepartment(){
    string message;

    // Đã thay thế logic selectedIndex bằng selectedID để đồng bộ với cấu trúc của Employee và Menu.h
    if(departmentService.deleteDepartment(selectedID, message)){
        showNotification(message);
        selectedID = "";
    }
    else{
        showNotification(message);
    }

    showConfirmDialog = false;
}

// POSITION MANAGEMENT
void Menu::drawPositionManagement(){
    drawHeader("QUAN LY CHUC VU");
    drawPanel(Rectangle{280, 120, 1050, 700}, COLOR_WHITE);

    drawText("DANH SACH CHUC VU", 310, 145, 24, COLOR_PRIMARY);

    if(drawButton(Rectangle{1050, 135, 230, 45}, "THEM CHUC VU", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        selectedTab = 1;
        positionDepartmentInput = "";
        positionNameInput = "";
        positionTypeInput = "Van phong";
        positionCoefficientInput = "1.0";
    }

    drawInputBox(Rectangle{310, 205, 400, 45}, searchText, "Tim theo ten chuc vu", false);

    vector<Position> positions = positionService.getAllPositions();
    vector<Position> filtered;

    for(const Position& position : positions){
        string name = position.getPositionName();

        if(searchText.empty() || name.find(searchText) != string::npos){
            filtered.push_back(position);
        }
    }

    DrawRectangle(310, 275, 950, 45, COLOR_BORDER);

    drawText("MA PHONG", 330, 288, 18, COLOR_PRIMARY);
    drawText("TEN CHUC VU", 490, 288, 18, COLOR_PRIMARY);
    drawText("LOAI NV", 760, 288, 18, COLOR_PRIMARY);
    drawText("HE SO", 920, 288, 18, COLOR_PRIMARY);

    int startY = 330;

    for(int i = 0; i < (int)filtered.size(); i++){
        int y = startY + i * 55;

        if(y > 770) break;

        DrawRectangle(310, y, 950, 50, i % 2 == 0 ? COLOR_WHITE : Fade(COLOR_BORDER, 0.5f));

        drawText(filtered[i].getPHG_ID(), 330, y + 15, 18, COLOR_TEXT);
        drawText(filtered[i].getPositionName(), 490, y + 15, 18, COLOR_TEXT);
        drawText(filtered[i].getEmployeeType(), 760, y + 15, 18, COLOR_TEXT);
        drawText(TextFormat("%.2f", filtered[i].getSalaryCoefficient()), 920, y + 15, 18, COLOR_TEXT);

        if(drawButton(Rectangle{1050, (float)y + 5, 85, 38}, "SUA", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            selectedTab = 2;
            selectedID = filtered[i].getPHG_ID() + "|" + filtered[i].getPositionName();

            positionDepartmentInput = filtered[i].getPHG_ID();
            positionNameInput = filtered[i].getPositionName();
            positionTypeInput = filtered[i].getEmployeeType();
            positionCoefficientInput = to_string(filtered[i].getSalaryCoefficient());
        }

        if(drawButton(Rectangle{1150, (float)y + 5, 85, 38}, "XOA", COLOR_DANGER, RED)){
            selectedID = filtered[i].getPHG_ID() + "|" + filtered[i].getPositionName();
            confirmAction = 4;
            showConfirmDialog = true;
        }
    }

    // Form them chuc vu
    if(selectedTab == 1){
        drawPanel(Rectangle{420, 270, 620, 390}, COLOR_WHITE);

        drawText("THEM CHUC VU", 630, 295, 24, COLOR_PRIMARY);

        drawInputBox(Rectangle{470, 350, 500, 45}, positionDepartmentInput, "Ma phong ban", false);

        drawInputBox(Rectangle{470, 410, 500, 45}, positionNameInput, "Ten chuc vu", false);

        drawInputBox(Rectangle{470, 470, 500, 45}, positionTypeInput, "Loai nhan vien (Cong nhan / Van phong)", false);

        drawInputBox(Rectangle{470, 530, 500, 45}, positionCoefficientInput, "He so luong", false);

        if(drawButton(Rectangle{470, 590, 220, 45}, "LUU", COLOR_SUCCESS, DARKGREEN)){
            handleAddPosition();
        }

        if(drawButton(Rectangle{740, 590, 220, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }

    // Form cap nhat chuc vu
    if(selectedTab == 2){
        drawPanel(Rectangle{420, 270, 620, 390}, COLOR_WHITE);

        drawText("CAP NHAT CHUC VU", 610, 295, 24, COLOR_PRIMARY);

        drawText("Ma phong ban: " + positionDepartmentInput, 470, 350, 20, COLOR_GRAY);

        drawInputBox(Rectangle{470, 400, 500, 45}, positionNameInput, "Ten chuc vu moi", false);

        drawInputBox(Rectangle{470, 460, 500, 45}, positionTypeInput, "Loai nhan vien", false);

        drawInputBox(Rectangle{470, 520, 500, 45}, positionCoefficientInput, "He so luong", false);

        if(drawButton(Rectangle{470, 580, 220, 45}, "CAP NHAT", COLOR_SUCCESS, DARKGREEN)){
            handleUpdatePosition();
        }

        if(drawButton(Rectangle{740, 580, 220, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }
}

// ADD POSITION
void Menu::handleAddPosition(){
    double coefficient = toDouble(positionCoefficientInput);

    Position position(positionDepartmentInput, positionNameInput, positionTypeInput, coefficient);

    // Lưu ý: Đã xóa tham số màu sắc trong showNotification để khớp với định nghĩa
    if(positionService.addPosition(position)){
        showNotification("Them chuc vu thanh cong!");
        selectedTab = 0;
    }
    else{
        showNotification("Khong the them chuc vu!");
    }
}

// UPDATE POSITION
void Menu::handleUpdatePosition(){
    size_t delimiterPos = selectedID.find('|');
    if(delimiterPos == string::npos) return;

    string oldPHG_ID = selectedID.substr(0, delimiterPos);
    string oldPositionName = selectedID.substr(delimiterPos + 1);

    double coefficient = toDouble(positionCoefficientInput);

    Position newPosition(positionDepartmentInput, positionNameInput, positionTypeInput, coefficient);

    // Lưu ý: Đã xóa tham số màu sắc trong showNotification và thay thế selectedIndex bằng selectedID
    if(positionService.updatePosition(oldPHG_ID, oldPositionName, newPosition)){
        showNotification("Cap nhat chuc vu thanh cong!");
        selectedTab = 0;
    }
    else{
        showNotification("Khong the cap nhat chuc vu!");
    }
}

// DELETE POSITION
void Menu::handleDeletePosition(){
    size_t delimiterPos = selectedID.find('|');
    if(delimiterPos == string::npos) return;

    string phgID = selectedID.substr(0, delimiterPos);
    string positionName = selectedID.substr(delimiterPos + 1);

    if(positionService.deletePosition(phgID, positionName)){
        showNotification("Xoa chuc vu thanh cong!");
        selectedID = "";
    }
    else{
        showNotification("Khong the xoa chuc vu!");
    }

    showConfirmDialog = false;
}

// PRODUCT MANAGEMENT
void Menu::drawProductManagement(){
    drawHeader("QUAN LY SAN PHAM");
    drawPanel(Rectangle{280, 120, 1050, 700}, COLOR_WHITE);

    drawText("DANH SACH SAN PHAM", 310, 145, 24, COLOR_PRIMARY);

    if(drawButton(Rectangle{1050, 135, 230, 45}, "THEM SAN PHAM", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        selectedTab = 1;
        productIDInput = "";
        productNameInput = "";
        productPriceInput = "";
    }

    drawInputBox(Rectangle{310, 205, 400, 45}, searchText, "Tim theo ma hoac ten", false);

    // Lay toan bo san pham, bao gom ca san pham ngung kinh doanh
    vector<Product> products = productService.getAllProducts();
    vector<Product> filtered;

    for(const Product& product : products){
        string id = product.getProductID();
        string name = product.getProductName();

        if(searchText.empty() || id.find(searchText) != string::npos || name.find(searchText) != string::npos){
            filtered.push_back(product);
        }
    }

    DrawRectangle(310, 275, 950, 45, COLOR_BORDER);

    drawText("MA SP", 330, 288, 18, COLOR_PRIMARY);
    drawText("TEN SAN PHAM", 480, 288, 18, COLOR_PRIMARY);
    drawText("DON GIA", 830, 288, 18, COLOR_PRIMARY);
    drawText("TRANG THAI", 970, 288, 18, COLOR_PRIMARY);

    int startY = 330;

    for(int i = 0; i < (int)filtered.size(); i++){
        int y = startY + i * 55;

        if(y > 770) break;

        DrawRectangle(310, y, 950, 50, i % 2 == 0 ? COLOR_WHITE : Fade(COLOR_BORDER, 0.5f));

        drawText(filtered[i].getProductID(), 330, y + 15, 18, COLOR_TEXT);
        drawText(filtered[i].getProductName(), 480, y + 15, 18, COLOR_TEXT);
        drawText(TextFormat("%.0f", filtered[i].getUnitPrice()), 830, y + 15, 18, COLOR_TEXT);

        bool active = filtered[i].getStatus();

        drawText(active ? "Dang kinh doanh" : "Ngung kinh doanh", 970, y + 15, 16, active ? COLOR_SUCCESS : COLOR_DANGER);

        if(drawButton(Rectangle{1100, (float)y + 5, 75, 38}, "SUA", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            selectedTab = 2;
            selectedID = filtered[i].getProductID();
            productIDInput = filtered[i].getProductID();
            productNameInput = filtered[i].getProductName();
            productPriceInput = to_string(filtered[i].getUnitPrice());
        }

        if(active){
            if(drawButton(Rectangle{1185, (float)y + 5, 75, 38}, "NGUNG", COLOR_WARNING, ORANGE)){
                selectedID = filtered[i].getProductID();
                confirmAction = 5;
                showConfirmDialog = true;
            }
        }
        else{
            if(drawButton(Rectangle{1185, (float)y + 5, 75, 38}, "MO LAI", COLOR_SUCCESS, DARKGREEN)){
                selectedID = filtered[i].getProductID();
                confirmAction = 6;
                showConfirmDialog = true;
            }
        }
    }

    // Form them san pham
    if(selectedTab == 1){
        drawPanel(Rectangle{420, 300, 620, 330}, COLOR_WHITE);

        drawText("THEM SAN PHAM", 630, 325, 24, COLOR_PRIMARY);

        drawInputBox(Rectangle{470, 390, 500, 45}, productNameInput, "Ten san pham", false);

        drawInputBox(Rectangle{470, 450, 500, 45}, productPriceInput, "Don gia", false);

        if(drawButton(Rectangle{470, 530, 220, 45}, "LUU", COLOR_SUCCESS, DARKGREEN)){
            handleAddProduct();
        }

        if(drawButton(Rectangle{740, 530, 220, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }

    // Form cap nhat san pham
    if(selectedTab == 2){
        drawPanel(Rectangle{420, 300, 620, 330}, COLOR_WHITE);

        drawText("CAP NHAT SAN PHAM", 600, 325, 24, COLOR_PRIMARY);

        drawText("Ma san pham: " + productIDInput, 470, 390, 20, COLOR_GRAY);

        drawInputBox(Rectangle{470, 440, 500, 45}, productNameInput, "Ten san pham", false);

        drawInputBox(Rectangle{470, 500, 500, 45}, productPriceInput, "Don gia", false);

        if(drawButton(Rectangle{470, 570, 220, 45}, "CAP NHAT", COLOR_SUCCESS, DARKGREEN)){
            handleUpdateProduct();
        }

        if(drawButton(Rectangle{740, 570, 220, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }
}

// ADD PRODUCT
void Menu::handleAddProduct(){
    double price = toDouble(productPriceInput);

    Product product("", productNameInput, price, true);

    // Lưu ý: Đã xóa tham số màu sắc trong showNotification để khớp với định nghĩa
    if(productService.addProduct(product)){
        showNotification("Them san pham thanh cong!");
        selectedTab = 0;
    }
    else{
        showNotification("Khong the them san pham!");
    }
}

// UPDATE PRODUCT
void Menu::handleUpdateProduct(){
    Product product;

    // Thay thế inputText bằng các biến trạng thái tương ứng đã định nghĩa trong Menu.h
    if(!productService.findByID(productIDInput, product)){
        showNotification("Khong tim thay san pham!");
        return;
    }

    product.setProductName(productNameInput);
    product.setUnitPrice(toDouble(productPriceInput));

    if(productService.updateProduct(product)){
        showNotification("Cap nhat san pham thanh cong!");
        selectedTab = 0;
    }
    else{
        showNotification("Khong the cap nhat san pham!");
    }
}

// DEACTIVATE PRODUCT
void Menu::handleDeactivateProduct(){
    // Đã thay thế logic selectedIndex bằng selectedID và xóa tham số màu của showNotification
    if(productService.deactivateProduct(selectedID)){
        showNotification("Da ngung kinh doanh san pham!");
    }
    else{
        showNotification("Khong the ngung kinh doanh!");
    }

    showConfirmDialog = false;
}

// ACTIVATE PRODUCT
void Menu::handleActivateProduct(){
    if(productService.activateProduct(selectedID)){
        showNotification("Da kich hoat lai san pham!");
    }
    else{
        showNotification("Khong the kich hoat san pham!");
    }

    showConfirmDialog = false;
}

// PRODUCTION MANAGEMENT
void Menu::drawProductionManagement(){
    drawHeader("QUAN LY SAN LUONG");
    drawPanel(Rectangle{280, 120, 1050, 700}, COLOR_WHITE);

    drawText("DANH SACH SAN LUONG", 310, 145, 24, COLOR_PRIMARY);

    if(drawButton(Rectangle{1050, 135, 230, 45}, "THEM SAN LUONG", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        selectedTab = 1;
        productionEmployeeInput = "";
        productionProductInput = "";
        productionQuantityInput = "";
        productionMonthInput = "";
        productionYearInput = "";
    }

    drawInputBox(Rectangle{310, 205, 300, 45}, searchText, "Ma nhan vien", false);
    drawInputBox(Rectangle{630, 205, 130, 45}, inputText[6], "Thang", false);
    drawInputBox(Rectangle{780, 205, 130, 45}, inputText[7], "Nam", false);

    vector<ProductionRecord> records = productionService.getAllRecords();
    vector<ProductionRecord> filtered;

    for(const ProductionRecord& record : records){
        bool match = true;

        if(!searchText.empty() && record.getNV_ID().find(searchText) == string::npos) match = false;
        if(!inputText[6].empty() && to_string(record.getMonth()) != inputText[6]) match = false;
        if(!inputText[7].empty() && to_string(record.getYear()) != inputText[7]) match = false;

        if(match){
            filtered.push_back(record);
        }
    }

    DrawRectangle(310, 275, 950, 45, COLOR_BORDER);

    drawText("MA BAN GHI", 325, 288, 17, COLOR_PRIMARY);
    drawText("MA NHAN VIEN", 465, 288, 17, COLOR_PRIMARY);
    drawText("MA SAN PHAM", 610, 288, 17, COLOR_PRIMARY);
    drawText("SO LUONG", 760, 288, 17, COLOR_PRIMARY);
    drawText("THANG/NAM", 900, 288, 17, COLOR_PRIMARY);

    int startY = 330;

    for(int i = 0; i < (int)filtered.size(); i++){
        int y = startY + i * 55;

        if(y > 770) break;

        const ProductionRecord& record = filtered[i];

        DrawRectangle(310, y, 950, 50, i % 2 == 0 ? COLOR_WHITE : Fade(COLOR_BORDER, 0.5f));

        drawText(record.getProductionID(), 325, y + 15, 16, COLOR_TEXT);
        drawText(record.getNV_ID(), 465, y + 15, 16, COLOR_TEXT);
        drawText(record.getProductID(), 610, y + 15, 16, COLOR_TEXT);
        drawText(TextFormat("%d", record.getQuantity()), 760, y + 15, 16, COLOR_TEXT);
        drawText(TextFormat("%02d/%d", record.getMonth(), record.getYear()), 900, y + 15, 16, COLOR_TEXT);

        if(drawButton(Rectangle{1050, (float)y + 5, 85, 38}, "SUA", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            selectedTab = 2;
            selectedID = record.getProductionID();
            inputText[0] = record.getProductionID();
            productionQuantityInput = to_string(record.getQuantity());
        }

        if(drawButton(Rectangle{1150, (float)y + 5, 85, 38}, "XOA", COLOR_DANGER, RED)){
            selectedID = record.getProductionID();
            confirmAction = 7;
            showConfirmDialog = true;
        }
    }

    // Form them san luong
    if(selectedTab == 1){
        drawPanel(Rectangle{400, 230, 660, 500}, COLOR_WHITE);

        drawText("THEM SAN LUONG", 620, 255, 24, COLOR_PRIMARY);

        drawInputBox(Rectangle{450, 320, 540, 45}, productionEmployeeInput, "Ma nhan vien", false);
        drawInputBox(Rectangle{450, 380, 540, 45}, productionProductInput, "Ma san pham", false);
        drawInputBox(Rectangle{450, 440, 540, 45}, productionQuantityInput, "So luong", false);
        drawInputBox(Rectangle{450, 500, 260, 45}, productionMonthInput, "Thang", false);
        drawInputBox(Rectangle{730, 500, 260, 45}, productionYearInput, "Nam", false);

        if(drawButton(Rectangle{450, 580, 220, 45}, "LUU", COLOR_SUCCESS, DARKGREEN)){
            handleAddProduction();
        }

        if(drawButton(Rectangle{720, 580, 220, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }

    // Form cap nhat san luong
    if(selectedTab == 2){
        drawPanel(Rectangle{400, 300, 660, 350}, COLOR_WHITE);

        drawText("CAP NHAT SAN LUONG", 590, 325, 24, COLOR_PRIMARY);

        drawText("Ma ban ghi: " + inputText[0], 450, 390, 20, COLOR_GRAY);

        drawInputBox(Rectangle{450, 440, 540, 45}, productionQuantityInput, "So luong moi", false);

        if(drawButton(Rectangle{450, 520, 220, 45}, "CAP NHAT", COLOR_SUCCESS, DARKGREEN)){
            handleUpdateProduction();
        }

        if(drawButton(Rectangle{720, 520, 220, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }
}

// ADD PRODUCTION
void Menu::handleAddProduction(){
    int quantity = toInt(productionQuantityInput);
    int month = toInt(productionMonthInput);
    int year = toInt(productionYearInput);

    string message;

    if(productionService.addProduction(productionEmployeeInput, productionProductInput, quantity, month, year, message)){
        showNotification(message);
        selectedTab = 0;
    }
    else{
        showNotification(message);
    }
}

// UPDATE PRODUCTION
void Menu::handleUpdateProduction(){
    int quantity = toInt(productionQuantityInput);
    string message;

    if(productionService.updateProduction(selectedID, quantity, message)){
        showNotification(message);
        selectedTab = 0;
    }
    else{
        showNotification(message);
    }
}

// DELETE PRODUCTION
void Menu::handleDeleteProduction(){
    string message;

    // Đã thay thế logic tìm kiếm qua selectedIndex bằng selectedID và xóa tham số màu sắc
    if(productionService.deleteProduction(selectedID, message)){
        showNotification(message);
        selectedID = "";
    }
    else{
        showNotification(message);
    }

    showConfirmDialog = false;
}

// SALARY MANAGEMENT
void Menu::drawSalaryManagement(){
    drawHeader("QUAN LY BANG LUONG");
    drawPanel(Rectangle{280, 120, 1050, 700}, COLOR_WHITE);

    drawText("DANH SACH BANG LUONG", 310, 145, 24, COLOR_PRIMARY);

    if(drawButton(Rectangle{1050, 135, 230, 45}, "TAO BANG LUONG", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        selectedTab = 1;
        salaryEmployeeInput = "";
        salaryMonthInput = "";
        salaryYearInput = "";
        salaryBaseInput = "0";
        salaryAllowanceInput = "0";
        salaryBonusInput = "0";
        salaryDeductionInput = "0";
    }

    drawInputBox(Rectangle{310, 205, 160, 45}, inputText[7], "Thang", false);
    drawInputBox(Rectangle{490, 205, 160, 45}, inputText[8], "Nam", false);

    vector<Salary> salaries = salaryService.getAllSalaries();
    vector<Salary> filtered;

    for(const Salary& salary : salaries){
        bool match = true;

        if(!inputText[7].empty() && to_string(salary.getMonth()) != inputText[7]) match = false;
        if(!inputText[8].empty() && to_string(salary.getYear()) != inputText[8]) match = false;

        if(match){
            filtered.push_back(salary);
        }
    }

    DrawRectangle(310, 275, 950, 45, COLOR_BORDER);

    drawText("MA LUONG", 325, 288, 17, COLOR_PRIMARY);
    drawText("MA NV", 470, 288, 17, COLOR_PRIMARY);
    drawText("THANG/NAM", 600, 288, 17, COLOR_PRIMARY);
    drawText("TONG LUONG", 760, 288, 17, COLOR_PRIMARY);

    int startY = 330;

    for(int i = 0; i < (int)filtered.size(); i++){
        int y = startY + i * 55;

        if(y > 770) break;

        const Salary& salary = filtered[i];

        DrawRectangle(310, y, 950, 50, i % 2 == 0 ? COLOR_WHITE : Fade(COLOR_BORDER, 0.5f));

        drawText(salary.getSalaryID(), 325, y + 15, 17, COLOR_TEXT);
        drawText(salary.getNV_ID(), 470, y + 15, 17, COLOR_TEXT);
        drawText(TextFormat("%02d/%d", salary.getMonth(), salary.getYear()), 600, y + 15, 17, COLOR_TEXT);
        drawText(TextFormat("%.0f", salary.getTotalSalary()), 760, y + 15, 17, COLOR_TEXT);

        if(drawButton(Rectangle{1080, (float)y + 5, 150, 38}, "CHI TIET", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            selectedID = salary.getSalaryID();
            selectedTab = 2;
        }
    }

    // Form tao bang luong
    if(selectedTab == 1){
        drawPanel(Rectangle{380, 190, 700, 570}, COLOR_WHITE);

        drawText("TAO BANG LUONG", 600, 215, 24, COLOR_PRIMARY);

        drawInputBox(Rectangle{430, 275, 280, 45}, salaryEmployeeInput, "Ma nhan vien", false);
        drawInputBox(Rectangle{730, 275, 280, 45}, salaryMonthInput, "Thang", false);
        drawInputBox(Rectangle{430, 335, 280, 45}, salaryYearInput, "Nam", false);
        drawInputBox(Rectangle{730, 335, 280, 45}, salaryBaseInput, "Luong co ban (NV van phong)", false);
        drawInputBox(Rectangle{430, 395, 280, 45}, salaryAllowanceInput, "Phu cap", false);
        drawInputBox(Rectangle{730, 395, 280, 45}, salaryBonusInput, "Thuong", false);
        drawInputBox(Rectangle{430, 455, 280, 45}, salaryDeductionInput, "Khau tru", false);

        drawText("Cong nhan: luong san pham tu dong tinh.", 430, 525, 18, COLOR_GRAY);
        drawText("Van phong: nhap luong co ban.", 430, 555, 18, COLOR_GRAY);

        if(drawButton(Rectangle{430, 620, 250, 45}, "TAO BANG LUONG", COLOR_SUCCESS, DARKGREEN)){
            handleCreateSalary();
        }

        if(drawButton(Rectangle{730, 620, 250, 45}, "HUY", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
        }
    }

    // Chi tiet bang luong
    if(selectedTab == 2){
        Salary selectedSalary;
        bool found = false;

        for(const Salary& salary : filtered){
            if(salary.getSalaryID() == selectedID){
                selectedSalary = salary;
                found = true;
                break;
            }
        }

        if(found){
            drawPanel(Rectangle{400, 250, 650, 440}, COLOR_WHITE);

            drawText("CHI TIET BANG LUONG", 590, 275, 24, COLOR_PRIMARY);

            drawText("Ma luong: " + selectedSalary.getSalaryID(), 450, 340, 20, COLOR_TEXT);
            drawText("Ma nhan vien: " + selectedSalary.getNV_ID(), 450, 380, 20, COLOR_TEXT);
            drawText(TextFormat("Luong co ban: %.0f", selectedSalary.getBaseSalary()), 450, 420, 20, COLOR_TEXT);
            drawText(TextFormat("Phu cap: %.0f", selectedSalary.getAllowance()), 450, 460, 20, COLOR_TEXT);
            drawText(TextFormat("Thuong: %.0f", selectedSalary.getBonus()), 450, 500, 20, COLOR_TEXT);
            drawText(TextFormat("Khau tru: %.0f", selectedSalary.getDeduction()), 450, 540, 20, COLOR_TEXT);
            drawText(TextFormat("TONG LUONG: %.0f", selectedSalary.getTotalSalary()), 450, 590, 24, COLOR_PRIMARY);

            if(drawButton(Rectangle{600, 630, 220, 45}, "DONG", COLOR_GRAY, DARKGRAY)){
                selectedTab = 0;
            }
        }
    }
}

// CREATE SALARY
void Menu::handleCreateSalary(){
    string NV_ID = salaryEmployeeInput;
    int month = toInt(salaryMonthInput);
    int year = toInt(salaryYearInput);
    double baseSalary = toDouble(salaryBaseInput);
    double allowance = toDouble(salaryAllowanceInput);
    double bonus = toDouble(salaryBonusInput);
    double deduction = toDouble(salaryDeductionInput);

    string message;

    // Tim nhan vien
    Employee employee;

    if(!employeeService.findByID(NV_ID, employee)){
        showNotification("Khong tim thay nhan vien!");
        return;
    }

    // Lay loai nhan vien tu chuc vu
    Position position;

    if(!positionService.findPosition(employee.getPHG_ID(), employee.getPosition(), position)){
        showNotification("Khong tim thay chuc vu cua nhan vien!");
        return;
    }

    double salaryCoefficient = position.getSalaryCoefficient();

    // Cong nhan: lay tien san luong trong thang
    if(position.getEmployeeType() == "Cong nhan"){
        baseSalary = productionService.calculateWorkerProductionAmount(NV_ID, month, year);
        salaryCoefficient = 0;
    }

    if(salaryService.createSalary(NV_ID, month, year, baseSalary, allowance, bonus, deduction, message)){
        showNotification(message);
        selectedTab = 0;
    }
    else{
        showNotification(message);
    }
}

// LEAVE MANAGEMENT
void Menu::drawLeaveManagement(){
    drawHeader("QUAN LY DON XIN NGHI PHEP");
    drawPanel(Rectangle{280, 120, 1050, 700}, COLOR_WHITE);

    drawText("DANH SACH DON XIN NGHI PHEP", 310, 145, 24, COLOR_PRIMARY);

    // Tim kiem theo ma nhan vien
    drawInputBox(Rectangle{310, 205, 300, 45}, searchText, "Ma nhan vien", false);

    // Loc theo trang thai
    const char* statuses[] = {"Tat ca", "Cho duyet", "Da duyet", "Tu choi", "Da huy"};

    if(drawButton(Rectangle{640, 205, 110, 45}, statuses[0], selectedTab == 0 ? COLOR_PRIMARY : COLOR_GRAY, COLOR_PRIMARY_HOVER)) selectedTab = 0;
    if(drawButton(Rectangle{760, 205, 110, 45}, statuses[1], selectedTab == 1 ? COLOR_PRIMARY : COLOR_GRAY, COLOR_PRIMARY_HOVER)) selectedTab = 1;
    if(drawButton(Rectangle{880, 205, 110, 45}, statuses[2], selectedTab == 2 ? COLOR_PRIMARY : COLOR_GRAY, COLOR_PRIMARY_HOVER)) selectedTab = 2;
    if(drawButton(Rectangle{1000, 205, 110, 45}, statuses[3], selectedTab == 3 ? COLOR_PRIMARY : COLOR_GRAY, COLOR_PRIMARY_HOVER)) selectedTab = 3;
    if(drawButton(Rectangle{1120, 205, 110, 45}, statuses[4], selectedTab == 4 ? COLOR_PRIMARY : COLOR_GRAY, COLOR_PRIMARY_HOVER)) selectedTab = 4;

    vector<LeaveRequest> requests = leaveService.getAllRequests();
    vector<LeaveRequest> filtered;

    for(const LeaveRequest& request : requests){
        bool match = true;

        if(!searchText.empty() && request.getNV_ID().find(searchText) == string::npos) match = false;
        if(selectedTab == 1 && request.getStatus() != "Cho duyet") match = false;
        if(selectedTab == 2 && request.getStatus() != "Da duyet") match = false;
        if(selectedTab == 3 && request.getStatus() != "Tu choi") match = false;
        if(selectedTab == 4 && request.getStatus() != "Da huy") match = false;

        if(match){
            filtered.push_back(request);
        }
    }

    // Tieu de bang
    DrawRectangle(310, 275, 950, 45, COLOR_BORDER);

    drawText("MA DON", 325, 288, 17, COLOR_PRIMARY);
    drawText("MA NHAN VIEN", 460, 288, 17, COLOR_PRIMARY);
    drawText("TU NGAY", 620, 288, 17, COLOR_PRIMARY);
    drawText("DEN NGAY", 760, 288, 17, COLOR_PRIMARY);
    drawText("TRANG THAI", 900, 288, 17, COLOR_PRIMARY);

    int startY = 330;

    for(int i = 0; i < (int)filtered.size(); i++){
        int y = startY + i * 55;

        if(y > 770) break;

        const LeaveRequest& request = filtered[i];

        DrawRectangle(310, y, 950, 50, i % 2 == 0 ? COLOR_WHITE : Fade(COLOR_BORDER, 0.5f));

        drawText(request.getRequestID(), 325, y + 15, 16, COLOR_TEXT);
        drawText(request.getNV_ID(), 460, y + 15, 16, COLOR_TEXT);
        drawText(request.getStartDate(), 620, y + 15, 16, COLOR_TEXT);
        drawText(request.getEndDate(), 760, y + 15, 16, COLOR_TEXT);

        bool pending = request.getStatus() == "Cho duyet";

        drawText(request.getStatus(), 900, y + 15, 16, pending ? COLOR_WARNING : COLOR_GRAY);

        if(drawButton(Rectangle{1110, (float)y + 5, 120, 38}, "CHI TIET", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            selectedID = request.getRequestID();
            selectedTab = 5;
        }
    }

    // Chi tiet don
    if(selectedTab == 5){
        LeaveRequest selectedRequest;
        bool found = false;

        for(const LeaveRequest& request : filtered){
            if(request.getRequestID() == selectedID){
                selectedRequest = request;
                found = true;
                break;
            }
        }

        if(found){
            drawPanel(Rectangle{390, 220, 680, 530}, COLOR_WHITE);

            drawText("CHI TIET DON XIN NGHI PHEP", 555, 245, 23, COLOR_PRIMARY);

            drawText("Ma don: " + selectedRequest.getRequestID(), 440, 310, 19, COLOR_TEXT);
            drawText("Ma nhan vien: " + selectedRequest.getNV_ID(), 440, 350, 19, COLOR_TEXT);
            drawText("Tu ngay: " + selectedRequest.getStartDate(), 440, 390, 19, COLOR_TEXT);
            drawText("Den ngay: " + selectedRequest.getEndDate(), 440, 430, 19, COLOR_TEXT);

            drawText("Ly do:", 440, 470, 19, COLOR_PRIMARY);
            drawText(selectedRequest.getReason(), 440, 500, 18, COLOR_TEXT);

            drawText("Trang thai: " + selectedRequest.getStatus(), 440, 540, 19, COLOR_PRIMARY);

            drawText("Ghi chu cua quan ly:", 440, 575, 18, COLOR_PRIMARY);

            drawInputBox(Rectangle{440, 605, 570, 45}, managerNoteInput, "Nhap ghi chu", false);

            if(selectedRequest.getStatus() == "Cho duyet"){
                if(drawButton(Rectangle{440, 675, 200, 45}, "DUYET DON", COLOR_SUCCESS, DARKGREEN)){
                    confirmAction = 8;
                    showConfirmDialog = true;
                }

                if(drawButton(Rectangle{680, 675, 200, 45}, "TU CHOI", COLOR_DANGER, RED)){
                    confirmAction = 9;
                    showConfirmDialog = true;
                }
            }

            if(drawButton(Rectangle{900, 675, 110, 45}, "DONG", COLOR_GRAY, DARKGRAY)){
                selectedTab = 0;
                managerNoteInput = "";
            }
        }
    }
}

// APPROVE LEAVE
void Menu::handleApproveLeave(){
    string message;

    // Đã thay thế selectedIndex bằng selectedID và inputText[1] bằng managerNoteInput
    if(leaveService.approveRequest(selectedID, "manager", managerNoteInput, message)){
        showNotification(message);
        selectedTab = 0;
        managerNoteInput = "";
    }
    else{
        showNotification(message);
    }

    showConfirmDialog = false;
}

// REJECT LEAVE
void Menu::handleRejectLeave(){
    string message;

    // Tương tự như duyệt đơn, thay thế selectedIndex bằng selectedID và inputText[1] bằng managerNoteInput
    if(leaveService.rejectRequest(selectedID, "manager", managerNoteInput, message)){
        showNotification(message);
        selectedTab = 0;
        managerNoteInput = "";
    }
    else{
        showNotification(message);
    }

    showConfirmDialog = false;
}

// STATISTICS MANAGEMENT
void Menu::drawStatistics(){
    drawHeader("THONG KE TONG QUAN");

    // LAY THANG VA NAM
    time_t now = time(nullptr);
    tm* currentTime = localtime(&now);

    int currentMonth = currentTime->tm_mon + 1;
    int currentYear = currentTime->tm_year + 1900;

    int month = currentMonth;
    int year = currentYear;

    if (!statMonthInput.empty() && isNumber(statMonthInput))
        month = toInt(statMonthInput);

    if (!statYearInput.empty() && isNumber(statYearInput))
        year = toInt(statYearInput);

    bool validPeriod = (month >= 1 && month <= 12 && year >= 2000);

    // LAY DU LIEU
    vector<Employee> employees = employeeService.getAllEmployees();
    vector<Department> departments = departmentService.getAllDepartments();
    vector<Salary> salaries = salaryService.getAllSalaries();
    vector<ProductionRecord> productions = productionService.getAllRecords();

    // TONG HOP SAN LUONG VA TIEN LUONG THEO 12 THANG
    vector<double> monthlyProduction(12, 0);
    vector<double> monthlySalary(12, 0);

    if(year >= 2000){
        for(const ProductionRecord& record : productions){
            if(record.getYear() == year && record.getMonth() >= 1 && record.getMonth() <= 12){
                monthlyProduction[record.getMonth() - 1] += record.getQuantity();
            }
        }

        for(const Salary& salary : salaries){
            if(salary.getYear() == year && salary.getMonth() >= 1 && salary.getMonth() <= 12){
                monthlySalary[salary.getMonth() - 1] += salary.getTotalSalary();
            }
        }
    }

    // THONG KE NHAN VIEN
    int totalEmployees = employees.size();
    int workingEmployees = 0;
    int resignedEmployees = 0;

    for(const Employee& employee : employees){
        if(employee.getStatus() == "Dang lam"){
            workingEmployees++;
        }
        else if(employee.getStatus() == "Da nghi viec"){
            resignedEmployees++;
        }
    }

    // THONG KE LUONG
    int salaryCount = 0;
    double totalSalary = 0;

    if(validPeriod){
        for(const Salary& salary : salaries){
            if(salary.getMonth() == month && salary.getYear() == year){
                salaryCount++;
                totalSalary += salary.getTotalSalary();
            }
        }
    }

    // THONG KE SAN LUONG
    int totalProduction = 0;
    int productionRecordCount = 0;

    if(validPeriod){
        for(const ProductionRecord& record : productions){
            if(record.getMonth() == month && record.getYear() == year){
                totalProduction += record.getQuantity();
                productionRecordCount++;
            }
        }
    }

    // HIEN THI BO LOC
    drawText("THOI GIAN THONG KE", 280, 105, 20, COLOR_PRIMARY);
    drawInputBox(Rectangle{280, 140, 150, 45}, statMonthInput, "Thang", false);
    drawInputBox(Rectangle{450, 140, 150, 45}, statYearInput, "Nam", false);
    drawText("De trong de su dung thang hien tai", 620, 150, 16, COLOR_GRAY);

    if(!validPeriod){
        drawText("Thang hoac nam khong hop le!", 280, 200, 18, COLOR_DANGER);
    }

    // CAC THE THONG KE
    int cardY = 230;
    int cardWidth = 260;
    int cardHeight = 115;
    int gap = 25;

    // Tong nhan vien
    DrawRectangleRounded(Rectangle{280, (float)cardY, (float)cardWidth, (float)cardHeight}, 0.12f, 10, COLOR_BORDER);
    drawText("TONG NHAN VIEN", 300, cardY + 18, 17, COLOR_GRAY);
    drawText(to_string(totalEmployees), 300, cardY + 55, 30, COLOR_PRIMARY);

    // Nhan vien dang lam
    DrawRectangleRounded(Rectangle{280.0f + cardWidth + gap, (float)cardY, (float)cardWidth, (float)cardHeight}, 0.12f, 10, COLOR_BORDER);
    drawText("DANG LAM VIEC", 300 + cardWidth + gap, cardY + 18, 17, COLOR_GRAY);
    drawText(to_string(workingEmployees), 300 + cardWidth + gap, cardY + 55, 30, COLOR_SUCCESS);

    // Nhan vien da nghi
    DrawRectangleRounded(Rectangle{280.0f + 2 * (cardWidth + gap), (float)cardY, (float)cardWidth, (float)cardHeight}, 0.12f, 10, COLOR_BORDER);
    drawText("DA NGHI VIEC", 300 + 2 * (cardWidth + gap), cardY + 18, 17, COLOR_GRAY);
    drawText(to_string(resignedEmployees), 300 + 2 * (cardWidth + gap), cardY + 55, 30, COLOR_DANGER);

    // THONG KE THEO THANG
    int secondRowY = 380;

    // So phong ban
    DrawRectangleRounded(Rectangle{280, (float)secondRowY, (float)cardWidth, (float)cardHeight}, 0.12f, 10, COLOR_BORDER);
    drawText("TONG PHONG BAN", 300, secondRowY + 18, 17, COLOR_GRAY);
    drawText(to_string(departments.size()), 300, secondRowY + 55, 30, COLOR_PRIMARY);

    // San luong
    DrawRectangleRounded(Rectangle{280.0f + cardWidth + gap, (float)secondRowY, (float)cardWidth, (float)cardHeight}, 0.12f, 10, COLOR_BORDER);
    drawText("SAN LUONG THANG", 300 + cardWidth + gap, secondRowY + 18, 17, COLOR_GRAY);
    drawText(validPeriod ? to_string(totalProduction) : "---", 300 + cardWidth + gap, secondRowY + 55, 30, COLOR_PRIMARY);

    // Tong luong
    DrawRectangleRounded(Rectangle{280.0f + 2 * (cardWidth + gap), (float)secondRowY, (float)cardWidth, (float)cardHeight}, 0.12f, 10, COLOR_BORDER);
    drawText("TONG LUONG DA LAP", 300 + 2 * (cardWidth + gap), secondRowY + 18, 17, COLOR_GRAY);
    drawText(validPeriod ? formatMoney(totalSalary) : "---", 300 + 2 * (cardWidth + gap), secondRowY + 55, 24, COLOR_PRIMARY);

    // BIEU DO THONG KE THEO THANG
    int chartY = 510;
    int chartWidth = 405;
    int chartHeight = 180;

    auto drawMonthlyChart = [&](int x, const string& title, const vector<double>& data, Color barColor, bool isSalary){
        drawPanel(Rectangle{static_cast<float>(x), static_cast<float>(chartY), static_cast<float>(chartWidth), static_cast<float>(chartHeight)}, COLOR_WHITE);
        drawText(title, x + 15, chartY + 10, 16, COLOR_PRIMARY);

        // Tim gia tri lon nhat
        double maxValue = 0;
        for(double value : data){
            if(value > maxValue) maxValue = value;
        }
        if(maxValue <= 0) maxValue = 1;

        float startX = static_cast<float>(x + 35);
        float chartBottom = static_cast<float>(chartY + 140);
        float maxBarHeight = 75.0f;
        float barWidth = 21.0f;
        float gap = 7.0f;

        // Ve truc ngang
        DrawLine(static_cast<int>(startX), static_cast<int>(chartBottom), x + chartWidth - 15, static_cast<int>(chartBottom), COLOR_BORDER);

        // Ve 12 cot
        for(int i = 0; i < 12; i++){
            float barX = startX + i * (barWidth + gap);
            float barHeight = static_cast<float>(data[i] / maxValue) * maxBarHeight;
            float barY = chartBottom - barHeight;

            if(data[i] > 0){
                DrawRectangleRounded(Rectangle{barX, barY, barWidth, barHeight}, 0.15f, 4, barColor);
            }

            string valueText = isSalary ? (to_string(static_cast<long long>(data[i] / 1000000)) + "tr") : to_string(static_cast<int>(data[i]));
            int valueY = static_cast<int>(data[i] > 0 ? barY - 17 : chartBottom - 15);

            drawText(valueText, static_cast<int>(barX) - 3, valueY, 9, COLOR_GRAY);
            drawText("T" + to_string(i + 1), static_cast<int>(barX), chartY + 148, 9, COLOR_GRAY);
        }
    };

    if(year >= 2000){
        drawMonthlyChart(280, "SAN LUONG THEO THANG - " + to_string(year), monthlyProduction, COLOR_PRIMARY, false);
        drawMonthlyChart(705, "LUONG THEO THANG - " + to_string(year), monthlySalary, COLOR_SUCCESS, true);
    }
    else{
        drawText("Nam khong hop le de hien thi bieu do!", 280, chartY + 40, 18, COLOR_DANGER);
    }
}

// MY PROFILE
void Menu::drawMyProfile(){
    drawHeader("HO SO CA NHAN");

    Employee employee;

    if(!employeeService.findByID(currentAccount.getNV_ID(), employee)){
        drawText("Khong tim thay ho so nhan vien!", 300, 180, 20, COLOR_DANGER);
        return;
    }

    drawPanel(Rectangle{280, 110, 850, 540}, COLOR_WHITE);

    drawText("THONG TIN CA NHAN", 310, 135, 22, COLOR_PRIMARY);

    int x1 = 320;
    int x2 = 700;
    int y = 190;
    int line = 48;

    drawText("Ma nhan vien:", x1, y, 18, COLOR_GRAY);
    drawText(employee.getNV_ID(), x2, y, 18, COLOR_TEXT);
    y += line;

    drawText("Ho va ten:", x1, y, 18, COLOR_GRAY);
    drawText(employee.getFullName(), x2, y, 18, COLOR_TEXT);
    y += line;

    drawText("Ngay sinh:", x1, y, 18, COLOR_GRAY);
    drawText(employee.getDateOfBirth(), x2, y, 18, COLOR_TEXT);
    y += line;

    drawText("Gioi tinh:", x1, y, 18, COLOR_GRAY);
    drawText(employee.getGender(), x2, y, 18, COLOR_TEXT);
    y += line;

    drawText("Phong ban:", x1, y, 18, COLOR_GRAY);
    drawText(getDepartmentName(employee.getPHG_ID()), x2, y, 18, COLOR_TEXT);
    y += line;

    drawText("Chuc vu:", x1, y, 18, COLOR_GRAY);
    drawText(employee.getPosition(), x2, y, 18, COLOR_TEXT);
    y += line;

    drawText("Ngay vao lam:", x1, y, 18, COLOR_GRAY);
    drawText(employee.getNgayVaoLam(), x2, y, 18, COLOR_TEXT);
    y += line;

    drawText("Trang thai:", x1, y, 18, COLOR_GRAY);
    drawText(employee.getStatus(), x2, y, 18, COLOR_TEXT);
    y += line;

    drawText("So dien thoai:", x1, y, 18, COLOR_GRAY);

    if(isEditingProfile){
        drawInputBox(Rectangle{(float)x2, (float)y - 8, 300, 38}, profilePhoneInput, "So dien thoai", false);
    }
    else{
        drawText(employee.getPhone(), x2, y, 18, COLOR_TEXT);
    }

    y += line;

    drawText("Dia chi:", x1, y, 18, COLOR_GRAY);

    if(isEditingProfile){
        drawInputBox(Rectangle{(float)x2, (float)y - 8, 350, 38}, profileAddressInput, "Dia chi", false);
    }
    else{
        drawText(employee.getAddress(), x2, y, 18, COLOR_TEXT);
    }

    y += 65;

    if(!isEditingProfile){
        if(drawButton(Rectangle{(float)x2, (float)y, 200, 45}, "Chinh sua thong tin", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            profilePhoneInput = employee.getPhone();
            profileAddressInput = employee.getAddress();
            isEditingProfile = true;
        }
    }
    else{
        if(drawButton(Rectangle{(float)x2, (float)y, 140, 45}, "Luu thay doi", COLOR_SUCCESS, DARKGREEN)){
            if(profilePhoneInput == employee.getPhone() && profileAddressInput == employee.getAddress()){
                showNotification("Khong co thong tin thay doi!");
                isEditingProfile = false;
            }
            else{
                confirmAction = 10;
                showConfirmDialog = true; // Lưu ý: showConfirmDialog trong Menu.h là bool, không phải hàm truyền tham số chuỗi
            }
        }

        if(drawButton(Rectangle{(float)x2 + 160, (float)y, 140, 45}, "Huy", COLOR_GRAY, DARKGRAY)){
            isEditingProfile = false;
            profilePhoneInput = "";
            profileAddressInput = "";
        }
    }
}

// UPDATE MY PROFILE
void Menu::handleUpdateMyProfile(){
    string message;

    // Đã gộp lời gọi hàm vào điều kiện if và xóa tham số màu của showNotification
    if(employeeService.updateMyContactInfo(currentAccount.getNV_ID(), profilePhoneInput, profileAddressInput, message)){
        showNotification("Cap nhat ho so thanh cong!");
        isEditingProfile = false;
        profilePhoneInput = "";
        profileAddressInput = "";
    }
    else{
        showNotification(message);
    }
}

// MY SALARY
void Menu::drawMySalary(){
    drawHeader("LICH SU LUONG CA NHAN");

    vector<Salary> salaries = salaryService.getEmployeeSalaryHistory(currentAccount.getNV_ID());

    drawPanel(Rectangle{280, 110, 850, 540}, COLOR_WHITE);

    drawText("MA NHAN VIEN: " + currentAccount.getNV_ID(), 310, 130, 18, COLOR_PRIMARY);
    drawText("LICH SU BANG LUONG", 310, 175, 20, COLOR_PRIMARY);

    // Khong co bang luong
    if(salaries.empty()){
        drawCenteredText("Chua co bang luong nao duoc tao.", 700, 350, 20, COLOR_GRAY);
        return;
    }

    // Tieu de bang
    int tableX = 310;
    int tableY = 220;
    int rowHeight = 45;

    drawText("STT", tableX, tableY, 16, COLOR_GRAY);
    drawText("THANG", tableX + 70, tableY, 16, COLOR_GRAY);
    drawText("NAM", tableX + 180, tableY, 16, COLOR_GRAY);
    drawText("TONG LUONG", tableX + 290, tableY, 16, COLOR_GRAY);
    drawText("CHI TIET", tableX + 540, tableY, 16, COLOR_GRAY);

    DrawLine(tableX, tableY + 30, tableX + 780, tableY + 30, COLOR_BORDER);

    for(int i = 0; i < (int)salaries.size(); i++){
        int y = tableY + 45 + i * rowHeight;

        drawText(to_string(i + 1), tableX, y, 16, COLOR_TEXT);
        drawText(to_string(salaries[i].getMonth()), tableX + 70, y, 16, COLOR_TEXT);
        drawText(to_string(salaries[i].getYear()), tableX + 180, y, 16, COLOR_TEXT);
        drawText(formatMoney(salaries[i].getTotalSalary()), tableX + 290, y, 16, COLOR_PRIMARY);

        if(drawButton(Rectangle{(float)tableX + 530, (float)y - 7, 120, 32}, "Xem chi tiet", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
            selectedID = salaries[i].getSalaryID();
            navigateTo(MenuScreen::MY_SALARY);
            selectedTab = 1;
        }
    }
}

// MY SALARY DETAIL
void Menu::drawMySalaryDetail(){
    drawHeader("CHI TIET BANG LUONG");

    Salary salary;

    // Thay thế selectedSalaryID bằng selectedID để đồng bộ với toàn hệ thống
    if(!salaryService.findSalaryByID(selectedID, salary)){
        drawText("Khong tim thay bang luong!", 310, 180, 20, COLOR_DANGER);

        if(drawButton(Rectangle{310, 230, 150, 40}, "Quay lai", COLOR_GRAY, DARKGRAY)){
            selectedTab = 0;
            navigateTo(MenuScreen::MY_SALARY);
        }

        return;
    }

    // Kiem tra quyen xem: chi xem duoc bang luong cua minh
    if(salary.getNV_ID() != currentAccount.getNV_ID()){
        drawText("Ban khong co quyen xem bang luong nay!", 310, 180, 20, COLOR_DANGER);
        return;
    }

    drawPanel(Rectangle{300, 120, 800, 500}, COLOR_WHITE);

    drawText("BANG LUONG THANG " + to_string(salary.getMonth()) + "/" + to_string(salary.getYear()), 340, 155, 23, COLOR_PRIMARY);

    int x1 = 350;
    int x2 = 720;
    int y = 230;
    int gap = 55;

    drawText("Ma bang luong:", x1, y, 18, COLOR_GRAY);
    drawText(salary.getSalaryID(), x2, y, 18, COLOR_TEXT);
    y += gap;

    drawText("Ma nhan vien:", x1, y, 18, COLOR_GRAY);
    drawText(salary.getNV_ID(), x2, y, 18, COLOR_TEXT);
    y += gap;

    drawText("Luong co ban:", x1, y, 18, COLOR_GRAY);
    drawText(formatMoney(salary.getBaseSalary()), x2, y, 18, COLOR_TEXT);
    y += gap;

    drawText("Phu cap:", x1, y, 18, COLOR_GRAY);
    drawText(formatMoney(salary.getAllowance()), x2, y, 18, COLOR_TEXT);
    y += gap;

    drawText("Thuong:", x1, y, 18, COLOR_GRAY);
    drawText(formatMoney(salary.getBonus()), x2, y, 18, COLOR_TEXT);
    y += gap;

    drawText("Khau tru:", x1, y, 18, COLOR_GRAY);
    drawText(formatMoney(salary.getDeduction()), x2, y, 18, COLOR_TEXT);
    y += gap;

    drawText("TONG LUONG:", x1, y, 20, COLOR_PRIMARY);
    drawText(formatMoney(salary.getTotalSalary()), x2, y, 22, COLOR_SUCCESS);

    // Cập nhật drawButton thay cho boolean (false) cũ
    if(drawButton(Rectangle{340, 560, 150, 42}, "Quay lai", COLOR_GRAY, DARKGRAY)){
        selectedTab = 0;
        navigateTo(MenuScreen::MY_SALARY);
    }
}

// MY LEAVE
void Menu::drawMyLeave(){
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const int centerX = screenWidth / 2;

    Rectangle panel = {250, 90, static_cast<float>(screenWidth - 290), static_cast<float>(screenHeight - 130)};

    DrawRectangleRounded(panel, 0.02f, 10, COLOR_WHITE);

    drawCenteredText("DON XIN NGHI PHEP", centerX, 110, 28, COLOR_PRIMARY);

    // Nut giao dien
    auto drawActionButton = [&](Rectangle bounds, const string& text, Color buttonColor) -> bool {
        Vector2 mouse = GetMousePosition();
        bool hovered = CheckCollisionPointRec(mouse, bounds);

        DrawRectangleRounded(bounds, 0.2f, 8, hovered ? Fade(buttonColor, 0.8f) : buttonColor);
        drawCenteredText(text, static_cast<int>(bounds.x + bounds.width / 2), static_cast<int>(bounds.y + 12), 16, COLOR_WHITE);

        return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    };

    // TAB 0: DANH SACH DON NGHI PHEP
    if(selectedTab == 0){
        if(drawActionButton(Rectangle{panel.x + 25, panel.y + 45, 160, 42}, "Tao don moi", COLOR_PRIMARY)){
            leaveStartDateInput = "";
            leaveEndDateInput = "";
            leaveReasonInput = "";
            selectedTab = 1;
        }

        vector<LeaveRequest> requests = leaveService.getRequestsByEmployee(currentAccount.getNV_ID());

        drawText("Ma nhan vien: " + currentAccount.getNV_ID(), static_cast<int>(panel.x + 25), static_cast<int>(panel.y + 110), 18, COLOR_GRAY);

        // Tieu de bang
        int tableY = static_cast<int>(panel.y + 155);

        DrawRectangle(static_cast<int>(panel.x + 20), tableY, static_cast<int>(panel.width - 40), 40, COLOR_PRIMARY);

        drawText("Ma don", static_cast<int>(panel.x + 35), tableY + 10, 15, COLOR_WHITE);
        drawText("Tu ngay", static_cast<int>(panel.x + 190), tableY + 10, 15, COLOR_WHITE);
        drawText("Den ngay", static_cast<int>(panel.x + 320), tableY + 10, 15, COLOR_WHITE);
        drawText("Trang thai", static_cast<int>(panel.x + 465), tableY + 10, 15, COLOR_WHITE);
        drawText("Chi tiet", static_cast<int>(panel.x + 620), tableY + 10, 15, COLOR_WHITE);

        int rowY = tableY + 45;

        if(requests.empty()){
            drawCenteredText("Ban chua co don xin nghi phep nao.", centerX, rowY + 30, 18, COLOR_GRAY);
        }
        else{
            for(const LeaveRequest& request : requests){
                Rectangle rowBounds = {panel.x + 20, static_cast<float>(rowY), panel.width - 40, 42};
                Vector2 mouse = GetMousePosition();
                bool hovered = CheckCollisionPointRec(mouse, rowBounds);

                DrawRectangleRec(rowBounds, hovered ? COLOR_ROW_HOVER : COLOR_ROW_EVEN);

                drawText(request.getRequestID(), panel.x + 35, rowY + 12, 15, COLOR_GRAY);
                drawText(request.getStartDate(), panel.x + 190, rowY + 12, 15, COLOR_GRAY);
                drawText(request.getEndDate(), panel.x + 320, rowY + 12, 15, COLOR_GRAY);
                drawText(request.getStatus(), panel.x + 465, rowY + 12, 15, COLOR_GRAY);
                drawText("Xem", panel.x + 620, rowY + 12, 15, COLOR_PRIMARY);

                if(hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    selectedID = request.getRequestID(); // Dong bo hoa thanh selectedID
                    selectedTab = 2;
                }

                rowY += 48;

                if(rowY > panel.y + panel.height - 30) break;
            }
        }
    }
    // TAB 1: TAO DON XIN NGHI PHEP
    else if(selectedTab == 1){
        drawText("Ngay bat dau (dd/MM/yyyy):", panel.x + 40, panel.y + 75, 17, COLOR_GRAY);
        drawInputBox(Rectangle{panel.x + 40, panel.y + 105, 300, 45}, leaveStartDateInput, "Ngay bat dau", false);

        drawText("Ngay ket thuc (dd/MM/yyyy):", panel.x + 40, panel.y + 170, 17, COLOR_GRAY);
        drawInputBox(Rectangle{panel.x + 40, panel.y + 200, 300, 45}, leaveEndDateInput, "Ngay ket thuc", false);

        drawText("Ly do nghi phep:", panel.x + 40, panel.y + 265, 17, COLOR_GRAY);
        drawInputBox(Rectangle{panel.x + 40, panel.y + 295, 550, 45}, leaveReasonInput, "Ly do", false);

        if(drawActionButton(Rectangle{panel.x + 40, panel.y + 380, 150, 45}, "Gui don", COLOR_PRIMARY)){
            string message;

            if(leaveService.createRequest(currentAccount.getNV_ID(), leaveStartDateInput, leaveEndDateInput, leaveReasonInput, message)){
                showNotification(message);
                leaveStartDateInput = "";
                leaveEndDateInput = "";
                leaveReasonInput = "";
                selectedTab = 0;
            }
            else{
                showNotification(message);
            }
        }

        if(drawActionButton(Rectangle{panel.x + 210, panel.y + 380, 150, 45}, "Quay lai", COLOR_GRAY)){
            leaveStartDateInput = "";
            leaveEndDateInput = "";
            leaveReasonInput = "";
            selectedTab = 0;
        }
    }
    // TAB 2: CHI TIET DON NGHI PHEP
    else if(selectedTab == 2){
        LeaveRequest request;

        if(!leaveService.findByID(selectedID, request) || request.getNV_ID() != currentAccount.getNV_ID()){ // Su dung selectedID
            drawCenteredText("Khong tim thay don nghi phep.", centerX, static_cast<int>(panel.y + 150), 20, COLOR_GRAY);

            if(drawActionButton(Rectangle{panel.x + 40, panel.y + 350, 150, 45}, "Quay lai", COLOR_GRAY)){
                selectedID = "";
                selectedTab = 0;
            }

            return;
        }

        int detailX = static_cast<int>(panel.x + 45);
        int detailY = static_cast<int>(panel.y + 75);
        int lineHeight = 45;

        drawText("Ma don: " + request.getRequestID(), detailX, detailY, 18, COLOR_GRAY);
        drawText("Ma nhan vien: " + request.getNV_ID(), detailX, detailY + lineHeight, 18, COLOR_GRAY);
        drawText("Ngay bat dau: " + request.getStartDate(), detailX, detailY + lineHeight * 2, 18, COLOR_GRAY);
        drawText("Ngay ket thuc: " + request.getEndDate(), detailX, detailY + lineHeight * 3, 18, COLOR_GRAY);
        drawText("Ly do: " + request.getReason(), detailX, detailY + lineHeight * 4, 18, COLOR_GRAY);
        drawText("Trang thai: " + request.getStatus(), detailX, detailY + lineHeight * 5, 18, COLOR_PRIMARY);
        drawText("Ghi chu quan ly: " + request.getManagerNote(), detailX, detailY + lineHeight * 6, 18, COLOR_GRAY);

        if(request.getStatus() == "Cho duyet"){
            if(drawActionButton(Rectangle{panel.x + 40, panel.y + 400, 150, 45}, "Huy don", COLOR_DANGER)){
                string message;

                if(leaveService.cancelRequest(request.getRequestID(), currentAccount.getNV_ID(), message)){
                    showNotification(message);
                    selectedID = "";
                    selectedTab = 0;
                }
                else{
                    showNotification(message);
                }
            }
        }

        if(drawActionButton(Rectangle{panel.x + 210, panel.y + 400, 150, 45}, "Quay lai", COLOR_GRAY)){
            selectedID = "";
            selectedTab = 0;
        }
    }
}

// CHANGE PASSWORD
void Menu::drawChangePassword(){
    int screenWidth = GetScreenWidth();
    int centerX = screenWidth / 2;

    Rectangle panel = {static_cast<float>(centerX - 300), 120, 600, 450};
    DrawRectangleRounded(panel, 0.03f, 10, COLOR_WHITE);

    drawCenteredText("DOI MAT KHAU", centerX, 150, 28, COLOR_PRIMARY);

    int labelX = static_cast<int>(panel.x + 50);
    int inputX = static_cast<int>(panel.x + 50);

    drawText("Mat khau hien tai:", labelX, 220, 17, COLOR_GRAY);
    drawInputBox(Rectangle{static_cast<float>(inputX), 250, 500, 45}, currentPasswordInput, "Mat khau hien tai", true);

    drawText("Mat khau moi:", labelX, 320, 17, COLOR_GRAY);
    drawInputBox(Rectangle{static_cast<float>(inputX), 350, 500, 45}, newPasswordInput, "Mat khau moi", true);

    drawText("Xac nhan mat khau moi:", labelX, 420, 17, COLOR_GRAY);
    drawInputBox(Rectangle{static_cast<float>(inputX), 450, 500, 45}, confirmPasswordInput, "Xac nhan mat khau", true);

    // Nut cap nhat
    if(drawButton(Rectangle{static_cast<float>(centerX - 180), 520, 160, 45}, "Cap nhat", COLOR_PRIMARY, COLOR_PRIMARY_HOVER)){
        handleChangePassword();
    }

    // Nut lam moi
    if(drawButton(Rectangle{static_cast<float>(centerX + 20), 520, 160, 45}, "Lam moi", COLOR_GRAY, COLOR_GRAY_HOVER)){
        currentPasswordInput = "";
        newPasswordInput = "";
        confirmPasswordInput = "";
    }
}

// CHANGE PASSWORD
void Menu::handleChangePassword(){
    string message;

    // Kiem tra cac truong nhap
    if(currentPasswordInput.empty() || newPasswordInput.empty() || confirmPasswordInput.empty()){
        showNotification("Vui long nhap day du thong tin!");
        return;
    }

    // Kiem tra mat khau xac nhan
    if(newPasswordInput != confirmPasswordInput){
        showNotification("Mat khau xac nhan khong khop!");
        return;
    }

    // Goi AccountService de doi mat khau
    if(accountService.changePassword(currentAccount.getNV_ID(), currentPasswordInput, newPasswordInput, message)){
        showNotification(message);
        currentPasswordInput = "";
        newPasswordInput = "";
        confirmPasswordInput = "";
    }
    else{
        showNotification(message);
    }
}