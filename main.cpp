#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // for rand()
#include <sstream>
#include <map>
#include <ctime> // for time()
#include <algorithm> // for sort
using namespace std;

// 用户结构体
struct User {
    string name;
    string account;
    string password;
};

// 管理员结构体
struct Admin {
    string account;
    string password;
};

// 从文件中加载用户信息
map<string, User> load_users(const string& filename) {
    map<string, User> users;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "打开文件失败: " << filename << endl;
        return users;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string name, account, password;
        iss >> name >> account >> password;
        users[account] = { name, account, password };
    }
    file.close();
    return users;
}

// 从文件中加载管理员信息
map<string, Admin> load_admins(const string& filename) {
    map<string, Admin> admins;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "打开文件失败: " << filename << endl;
        return admins;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string account, password;
        iss >> account >> password;
        admins[account] = { account, password };
    }
    file.close();
    return admins;
}

// 用户登录
bool user_login(const map<string, User>& users, User& loggedInUser) {
    cout << "请输入账号: ";
    cin >> loggedInUser.account;
    cout << "请输入密码: ";
    cin >> loggedInUser.password;
    auto it = users.find(loggedInUser.account);
    if (it != users.end() && it->second.password == loggedInUser.password) {
        loggedInUser.name = it->second.name;
        cout << "用户登录成功" << endl;
        return true;
    }
    cout << "无效的账户或密码" << endl;
    return false;
}

// 管理员登录
bool admin_login(const map<string, Admin>& admins, Admin& loggedInAdmin) {
    cout << "请输入账号: ";
    cin >> loggedInAdmin.account;
    cout << "请输入密码: ";
    cin >> loggedInAdmin.password;
    auto it = admins.find(loggedInAdmin.account);
    if (it != admins.end() && it->second.password == loggedInAdmin.password) {
        cout << "管理员登录成功" << endl;
        return true;
    }
    cout << "无效的账户或密码" << endl;
    return false;
}

// 菜色基类
class Dish {
protected:
    string name;
    string ingredients;
    string location;
    double price;
public:
    Dish(string n, string ing, string loc, double p) : name(n), ingredients(ing), location(loc), price(p) {}
    virtual void displayDetails() const {
        cout << "菜名: " << name << ", 配料: " << ingredients << ", 位置: " << location << ", 价格: ￥" << price << endl;
    }
    virtual ~Dish() {}
    string getName() const { return name; }
};

// 炒菜类
class StirFry : public Dish {
private:
    string taste;
public:
    StirFry(string n, string ing, string loc, double p, string t) : Dish(n, ing, loc, p), taste(t) {}
    void displayDetails() const override {
        cout << "菜名: " << name << ", 配料: " << ingredients << ", 位置: " << location << ", 价格: ￥" << price << ", 口味: " << taste << endl;
    }
};

// 粉面类
class Noodles : public Dish {
private:
    string toppings;
public:
    Noodles(string n, string ing, string loc, double p, string top) : Dish(n, ing, loc, p), toppings(top) {}
    void displayDetails() const override {
        cout << "菜名: " << name << ", 配料: " << ingredients << ", 位置: " << location << ", 价格: ￥" << price << ", 浇汁: " << toppings << endl;
    }
};

// 清真类
class Halal : public Dish {
private:
    string hatal;
public:
    Halal(string n, string ing, string loc, double p, string hat) : Dish(n, ing, loc, p), hatal(hat) {}
    void displayDetails() const override {
        cout << "菜名: " << name << ", 配料: " << ingredients << ", 位置: " << location << ", 价格: ￥" << price << ", 特色: " << hatal << endl;
    }
};

void view_all_dishes(const string& category) {
    ifstream dishesFile("dishes.txt");
    if (!dishesFile.is_open()) {
        cerr << "打开文件失败: dishes.txt" << endl;
        return;
    }
    string line;
    cout << "全部菜色:" << endl;
    bool found = false; // 用于检查是否找到匹配的菜品
    while (getline(dishesFile, line)) {
        istringstream iss(line);
        string type, name, ingredients, location, tasteOrToppings;
        double price;
        iss >> type >> name >> ingredients >> location >> price;
        getline(iss, tasteOrToppings);
        if (category == "全部" || category == type) {
            found = true;
            if (type == "炒菜") {
                StirFry stirFry(name, ingredients, location, price, tasteOrToppings);
                stirFry.displayDetails();
            }
            else if (type == "面食") {
                Noodles noodles(name, ingredients, location, price, tasteOrToppings);
                noodles.displayDetails();
            }
            else if (type == "清真") {
                Halal halal(name, ingredients, location, price, tasteOrToppings);
                halal.displayDetails();
            }
        }
    }
    if (!found) {
        cout << "未找到相应菜品: " << category << endl;
    }
    dishesFile.close();
}

// 添加新菜色
void add_new_dish() {
    string type, name, ingredients, location, tasteOrToppings;
    double price;
    cout << "请输入菜种 (炒菜/面食/清真): ";
    cin >> type;
    cout << "请输入菜名: ";
    cin >> name;
    cout << "请输入配料: ";
    cin.ignore();
    getline(cin, ingredients);
    cout << "请输入位置: ";
    getline(cin, location);
    cout << "请输入价格: ";
    cin >> price;
    ofstream dishesFile("dishes.txt", ios::app);
    dishesFile << type << " " << name << " " << ingredients << " " << location << " " << price;
    if (type == "炒菜") {
        cout << "请输入口味: ";
        cin >> tasteOrToppings;
        dishesFile << " " << tasteOrToppings;
    }
    else if (type == "面食" || type == "清真") {
        cout << "请输入浇汁/特色: ";
        cin >> tasteOrToppings;
        dishesFile << " " << tasteOrToppings;
    }
    dishesFile << endl;
    dishesFile.close();
    cout << "新菜色添加成功" << endl;
}

// 删除菜色
void delete_dish() {
    string name;
    cout << "请输入要删除的菜色: ";
    cin >> name;
    ifstream dishesFile("dishes.txt");
    ofstream tempFile("temp.txt");
    string line;
    bool found = false;
    while (getline(dishesFile, line)) {
        istringstream iss(line);
        string type, dishName, ingredients, location, tasteOrToppings;
        double price;
        iss >> type >> dishName >> ingredients >> location >> price;
        getline(iss, tasteOrToppings);
        if (dishName == name) {
            found = true;
        }
        else {
            tempFile << type << " " << dishName << " " << ingredients << " " << location << " " << price << " " << tasteOrToppings << endl;
        }
    }
    dishesFile.close();
    tempFile.close();
    remove("dishes.txt");
    rename("temp.txt", "dishes.txt");
    if (found) {
        cout << "成功删除菜色" << endl;
    }
    else {
        cout << "未找到菜色" << endl;
    }
}

// 提交意见
void submit_feedback() {
    string dishName, feedback;
    int rating;
    cout << "请输入菜名: ";
    cin >> dishName;
    cout << "请提交反馈: ";
    cin.ignore(); // 清除输入缓冲区
    getline(cin, feedback);
    cout << "请为该菜品打分 (1-5): ";
    cin >> rating;

    // 验证评分是否在有效范围
    if (rating < 1 || rating > 5) {
        cout << "无效评分，请输入1到5之间的数字。" << endl;
        return;
    }

    ofstream feedbackFile("comments.txt", ios::app);
    feedbackFile << dishName << " 反馈：" << feedback << " 评分：" << rating << endl;
    feedbackFile.close();
    cout << "反馈提交成功 期待您的意见！" << endl;
}

// 查看用户反馈
void view_feedback() {
    ifstream feedbackFile("comments.txt");
    string line;
    cout << "查看用户反馈:" << endl;
    while (getline(feedbackFile, line)) {
        cout << line << endl;
    }
    feedbackFile.close();
}

// 添加新管理员
void add_new_admin(map<string, Admin>& admins) {
    string account, password;
    cout << "请输入新的管理员账号 (8位数字): ";
    cin >> account;
    cout << "请输入新的管理员密码 (9位数字): ";
    cin >> password;
    admins[account] = { account, password };
    ofstream adminFile("admin.txt", ios::app);
    adminFile << account << " " << password << endl;
    adminFile.close();
    cout << "成功添加新管理员" << endl;
}

// 删除管理员
void delete_admin(map<string, Admin>& admins) {
    string account;
    cout << "请输入管理员账号: ";
    cin >> account;
    if (admins.erase(account)) {
        // 更新admin.txt文件
        ofstream adminFile("admin.txt");
        for (const auto& admin : admins) {
            adminFile << admin.second.account << " " << admin.second.password << endl;
        }
        adminFile.close();
        cout << "管理员删除成功" << endl;
    }
    else {
        cout << "未找到管理员" << endl;
    }
}

// 修改管理员密码
void change_admin_password(map<string, Admin>& admins) {
    string account, oldPassword, newPassword;
    cout << "请输入管理员账号: ";
    cin >> account;
    cout << "请输入原密码: ";
    cin >> oldPassword;
    auto it = admins.find(account);
    if (it != admins.end() && it->second.password == oldPassword) {
        cout << "请输入新密码: ";
        cin >> newPassword;
        it->second.password = newPassword;
        // 更新admin.txt文件
        ofstream adminFile("admin.txt");
        for (const auto& admin : admins) {
            adminFile << admin.second.account << " " << admin.second.password << endl;
        }
        adminFile.close();
        cout << "密码修改成功" << endl;
    }
    else {
        cout << "账号或密码无效" << endl;
    }
}

// 搜索菜色
void search_dish(const string& name) {
    ifstream dishesFile("dishes.txt");
    string line;
    bool found = false;
    while (getline(dishesFile, line)) {
        istringstream iss(line);
        string type, dishName, ingredients, location, tasteOrToppings;
        double price;
        iss >> type >> dishName >> ingredients >> location >> price;
        getline(iss, tasteOrToppings);
        if (dishName == name) {
            found = true;
            if (type == "炒菜") {
                StirFry stirFry(dishName, ingredients, location, price, tasteOrToppings);
                stirFry.displayDetails();
            }
            else if (type == "面食") {
                Noodles noodles(dishName, ingredients, location, price, tasteOrToppings);
                noodles.displayDetails();
            }
            else if (type == "清真") {
                Halal halal(dishName, ingredients, location, price, tasteOrToppings);
                halal.displayDetails();
            }
            break;
        }
    }
    dishesFile.close();
    if (!found) {
        cout << "未找到菜色" << endl;
    }
}

void search_dish() {
    string name;
    cout << "请输入菜名: ";
    cin >> name;
    search_dish(name);
}

// 推荐菜色
void recommend_dish() {
    ifstream feedbackFile("comments.txt");
    if (!feedbackFile.is_open()) {
        cerr << "打开文件失败: comments.txt" << endl;
        return;
    }

    map<string, vector<int>> dishRatings;
    string line;
    while (getline(feedbackFile, line)) {
        istringstream iss(line);
        string dishName, feedback;
        int rating;
        iss >> dishName;

        iss >> feedback;
        getline(iss, feedback, ' '); // 跳过 "评分："
        iss >> rating;
        dishRatings[dishName].push_back(rating);
    }
    feedbackFile.close();

    if (dishRatings.empty()) {
        cout << "无推荐菜色" << endl;
        return;
    }

    // 计算每个菜品的平均评分
    vector<pair<string, double>> averageRatings;
    for (const auto& dish : dishRatings) {
        double sum = 0;
        for (int rating : dish.second) {
            sum += rating;
        }
        double average = sum / dish.second.size();
        averageRatings.push_back({ dish.first, average });
    }

    // 按平均评分降序排序
    sort(averageRatings.begin(), averageRatings.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return b.second < a.second;
        });

    // 推荐评分最高的菜品
    string recommendedDish = averageRatings.front().first;
    cout << "推荐评分最高的菜品:" << endl;
    search_dish(recommendedDish);
}

int main() {
    map<string, User> users = load_users("users.txt");
    map<string, Admin> admins = load_admins("admin.txt");
    User loggedInUser;
    Admin loggedInAdmin;
    int choice;
    cout << "欢迎来到山青食堂管理系统!" << endl;
    while (true) {
        cout << "主菜单:" << endl;
        cout << "1. 用户登录" << endl;
        cout << "2. 管理员登录" << endl;
        cout << "3. 浏览所有菜品" << endl;
        cout << "4. 搜索菜品" << endl;
        cout << "5. 提交反馈" << endl;
        cout << "6. 推荐菜品" << endl; // 新增选项
        cout << "7. 退出" << endl;
        cout << "请输入选择: ";
        cin >> choice;
        switch (choice) {
        case 1:
            if (user_login(users, loggedInUser)) {
                int userChoice;
                while (true) {
                    cout << "用户菜单:" << endl;
                    cout << "1. 浏览所有菜品" << endl;
                    cout << "2. 按类查看菜品" << endl;
                    cout << "3. 搜索菜品" << endl;
                    cout << "4. 提交反馈" << endl;
                    cout << "5. 推荐菜品" << endl; // 新增选项
                    cout << "6. 退出登录" << endl;
                    cout << "请输入选择: ";
                    cin >> userChoice;
                    if (userChoice == 1) {
                        view_all_dishes("全部");
                    }
                    else if (userChoice == 2) {
                        cout << "请输入分类 (炒菜/面食/清真): ";
                        string category;
                        cin >> category;
                        view_all_dishes(category);
                    }
                    else if (userChoice == 3) {
                        search_dish();
                    }
                    else if (userChoice == 4) {
                        submit_feedback();
                    }
                    else if (userChoice == 5) {
                        recommend_dish(); // 调用新函数
                    }
                    else if (userChoice == 6) {
                        break;
                    }
                    else {
                        cout << "无效选择" << endl;
                    }
                }
            }
            break;
        case 2:
            if (admin_login(admins, loggedInAdmin)) {
                int adminChoice;
                while (true) {
                    cout << "管理员菜单:" << endl;
                    cout << "1. 添加新管理员" << endl;
                    cout << "2. 删除管理员" << endl;
                    cout << "3. 修改管理员密码" << endl;
                    cout << "4. 添加新菜色" << endl;
                    cout << "5. 删除菜色" << endl;
                    cout << "6. 浏览用户反馈" << endl;
                    cout << "7. 退出登录" << endl;
                    cout << "请输入选择: ";
                    cin >> adminChoice;
                    if (adminChoice == 1) {
                        add_new_admin(admins);
                    }
                    else if (adminChoice == 2) {
                        delete_admin(admins);
                    }
                    else if (adminChoice == 3) {
                        change_admin_password(admins);
                    }
                    else if (adminChoice == 4) {
                        add_new_dish();
                    }
                    else if (adminChoice == 5) {
                        delete_dish();
                    }
                    else if (adminChoice == 6) {
                        view_feedback();
                    }
                    else if (adminChoice == 7) {
                        break;
                    }
                    else {
                        cout << "无效选择" << endl;
                    }
                }
            }
            break;
        case 3:
            view_all_dishes("全部");
            break;
        case 4:
            search_dish();
            break;
        case 5:
            submit_feedback();
            break;
        case 6:
            recommend_dish(); // 调用新函数
            break;
        case 7:
            cout << "退出系统 再见!" << endl;
            return 0;
        default:
            cout << "无效选择" << endl;
        }
    }
    return 0;
}
