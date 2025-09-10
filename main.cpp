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

// �û��ṹ��
struct User {
    string name;
    string account;
    string password;
};

// ����Ա�ṹ��
struct Admin {
    string account;
    string password;
};

// ���ļ��м����û���Ϣ
map<string, User> load_users(const string& filename) {
    map<string, User> users;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "���ļ�ʧ��: " << filename << endl;
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

// ���ļ��м��ع���Ա��Ϣ
map<string, Admin> load_admins(const string& filename) {
    map<string, Admin> admins;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "���ļ�ʧ��: " << filename << endl;
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

// �û���¼
bool user_login(const map<string, User>& users, User& loggedInUser) {
    cout << "�������˺�: ";
    cin >> loggedInUser.account;
    cout << "����������: ";
    cin >> loggedInUser.password;
    auto it = users.find(loggedInUser.account);
    if (it != users.end() && it->second.password == loggedInUser.password) {
        loggedInUser.name = it->second.name;
        cout << "�û���¼�ɹ�" << endl;
        return true;
    }
    cout << "��Ч���˻�������" << endl;
    return false;
}

// ����Ա��¼
bool admin_login(const map<string, Admin>& admins, Admin& loggedInAdmin) {
    cout << "�������˺�: ";
    cin >> loggedInAdmin.account;
    cout << "����������: ";
    cin >> loggedInAdmin.password;
    auto it = admins.find(loggedInAdmin.account);
    if (it != admins.end() && it->second.password == loggedInAdmin.password) {
        cout << "����Ա��¼�ɹ�" << endl;
        return true;
    }
    cout << "��Ч���˻�������" << endl;
    return false;
}

// ��ɫ����
class Dish {
protected:
    string name;
    string ingredients;
    string location;
    double price;
public:
    Dish(string n, string ing, string loc, double p) : name(n), ingredients(ing), location(loc), price(p) {}
    virtual void displayDetails() const {
        cout << "����: " << name << ", ����: " << ingredients << ", λ��: " << location << ", �۸�: ��" << price << endl;
    }
    virtual ~Dish() {}
    string getName() const { return name; }
};

// ������
class StirFry : public Dish {
private:
    string taste;
public:
    StirFry(string n, string ing, string loc, double p, string t) : Dish(n, ing, loc, p), taste(t) {}
    void displayDetails() const override {
        cout << "����: " << name << ", ����: " << ingredients << ", λ��: " << location << ", �۸�: ��" << price << ", ��ζ: " << taste << endl;
    }
};

// ������
class Noodles : public Dish {
private:
    string toppings;
public:
    Noodles(string n, string ing, string loc, double p, string top) : Dish(n, ing, loc, p), toppings(top) {}
    void displayDetails() const override {
        cout << "����: " << name << ", ����: " << ingredients << ", λ��: " << location << ", �۸�: ��" << price << ", ��֭: " << toppings << endl;
    }
};

// ������
class Halal : public Dish {
private:
    string hatal;
public:
    Halal(string n, string ing, string loc, double p, string hat) : Dish(n, ing, loc, p), hatal(hat) {}
    void displayDetails() const override {
        cout << "����: " << name << ", ����: " << ingredients << ", λ��: " << location << ", �۸�: ��" << price << ", ��ɫ: " << hatal << endl;
    }
};

void view_all_dishes(const string& category) {
    ifstream dishesFile("dishes.txt");
    if (!dishesFile.is_open()) {
        cerr << "���ļ�ʧ��: dishes.txt" << endl;
        return;
    }
    string line;
    cout << "ȫ����ɫ:" << endl;
    bool found = false; // ���ڼ���Ƿ��ҵ�ƥ��Ĳ�Ʒ
    while (getline(dishesFile, line)) {
        istringstream iss(line);
        string type, name, ingredients, location, tasteOrToppings;
        double price;
        iss >> type >> name >> ingredients >> location >> price;
        getline(iss, tasteOrToppings);
        if (category == "ȫ��" || category == type) {
            found = true;
            if (type == "����") {
                StirFry stirFry(name, ingredients, location, price, tasteOrToppings);
                stirFry.displayDetails();
            }
            else if (type == "��ʳ") {
                Noodles noodles(name, ingredients, location, price, tasteOrToppings);
                noodles.displayDetails();
            }
            else if (type == "����") {
                Halal halal(name, ingredients, location, price, tasteOrToppings);
                halal.displayDetails();
            }
        }
    }
    if (!found) {
        cout << "δ�ҵ���Ӧ��Ʒ: " << category << endl;
    }
    dishesFile.close();
}

// ����²�ɫ
void add_new_dish() {
    string type, name, ingredients, location, tasteOrToppings;
    double price;
    cout << "��������� (����/��ʳ/����): ";
    cin >> type;
    cout << "���������: ";
    cin >> name;
    cout << "����������: ";
    cin.ignore();
    getline(cin, ingredients);
    cout << "������λ��: ";
    getline(cin, location);
    cout << "������۸�: ";
    cin >> price;
    ofstream dishesFile("dishes.txt", ios::app);
    dishesFile << type << " " << name << " " << ingredients << " " << location << " " << price;
    if (type == "����") {
        cout << "�������ζ: ";
        cin >> tasteOrToppings;
        dishesFile << " " << tasteOrToppings;
    }
    else if (type == "��ʳ" || type == "����") {
        cout << "�����뽽֭/��ɫ: ";
        cin >> tasteOrToppings;
        dishesFile << " " << tasteOrToppings;
    }
    dishesFile << endl;
    dishesFile.close();
    cout << "�²�ɫ��ӳɹ�" << endl;
}

// ɾ����ɫ
void delete_dish() {
    string name;
    cout << "������Ҫɾ���Ĳ�ɫ: ";
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
        cout << "�ɹ�ɾ����ɫ" << endl;
    }
    else {
        cout << "δ�ҵ���ɫ" << endl;
    }
}

// �ύ���
void submit_feedback() {
    string dishName, feedback;
    int rating;
    cout << "���������: ";
    cin >> dishName;
    cout << "���ύ����: ";
    cin.ignore(); // ������뻺����
    getline(cin, feedback);
    cout << "��Ϊ�ò�Ʒ��� (1-5): ";
    cin >> rating;

    // ��֤�����Ƿ�����Ч��Χ
    if (rating < 1 || rating > 5) {
        cout << "��Ч���֣�������1��5֮������֡�" << endl;
        return;
    }

    ofstream feedbackFile("comments.txt", ios::app);
    feedbackFile << dishName << " ������" << feedback << " ���֣�" << rating << endl;
    feedbackFile.close();
    cout << "�����ύ�ɹ� �ڴ����������" << endl;
}

// �鿴�û�����
void view_feedback() {
    ifstream feedbackFile("comments.txt");
    string line;
    cout << "�鿴�û�����:" << endl;
    while (getline(feedbackFile, line)) {
        cout << line << endl;
    }
    feedbackFile.close();
}

// ����¹���Ա
void add_new_admin(map<string, Admin>& admins) {
    string account, password;
    cout << "�������µĹ���Ա�˺� (8λ����): ";
    cin >> account;
    cout << "�������µĹ���Ա���� (9λ����): ";
    cin >> password;
    admins[account] = { account, password };
    ofstream adminFile("admin.txt", ios::app);
    adminFile << account << " " << password << endl;
    adminFile.close();
    cout << "�ɹ�����¹���Ա" << endl;
}

// ɾ������Ա
void delete_admin(map<string, Admin>& admins) {
    string account;
    cout << "���������Ա�˺�: ";
    cin >> account;
    if (admins.erase(account)) {
        // ����admin.txt�ļ�
        ofstream adminFile("admin.txt");
        for (const auto& admin : admins) {
            adminFile << admin.second.account << " " << admin.second.password << endl;
        }
        adminFile.close();
        cout << "����Աɾ���ɹ�" << endl;
    }
    else {
        cout << "δ�ҵ�����Ա" << endl;
    }
}

// �޸Ĺ���Ա����
void change_admin_password(map<string, Admin>& admins) {
    string account, oldPassword, newPassword;
    cout << "���������Ա�˺�: ";
    cin >> account;
    cout << "������ԭ����: ";
    cin >> oldPassword;
    auto it = admins.find(account);
    if (it != admins.end() && it->second.password == oldPassword) {
        cout << "������������: ";
        cin >> newPassword;
        it->second.password = newPassword;
        // ����admin.txt�ļ�
        ofstream adminFile("admin.txt");
        for (const auto& admin : admins) {
            adminFile << admin.second.account << " " << admin.second.password << endl;
        }
        adminFile.close();
        cout << "�����޸ĳɹ�" << endl;
    }
    else {
        cout << "�˺Ż�������Ч" << endl;
    }
}

// ������ɫ
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
            if (type == "����") {
                StirFry stirFry(dishName, ingredients, location, price, tasteOrToppings);
                stirFry.displayDetails();
            }
            else if (type == "��ʳ") {
                Noodles noodles(dishName, ingredients, location, price, tasteOrToppings);
                noodles.displayDetails();
            }
            else if (type == "����") {
                Halal halal(dishName, ingredients, location, price, tasteOrToppings);
                halal.displayDetails();
            }
            break;
        }
    }
    dishesFile.close();
    if (!found) {
        cout << "δ�ҵ���ɫ" << endl;
    }
}

void search_dish() {
    string name;
    cout << "���������: ";
    cin >> name;
    search_dish(name);
}

// �Ƽ���ɫ
void recommend_dish() {
    ifstream feedbackFile("comments.txt");
    if (!feedbackFile.is_open()) {
        cerr << "���ļ�ʧ��: comments.txt" << endl;
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
        getline(iss, feedback, ' '); // ���� "���֣�"
        iss >> rating;
        dishRatings[dishName].push_back(rating);
    }
    feedbackFile.close();

    if (dishRatings.empty()) {
        cout << "���Ƽ���ɫ" << endl;
        return;
    }

    // ����ÿ����Ʒ��ƽ������
    vector<pair<string, double>> averageRatings;
    for (const auto& dish : dishRatings) {
        double sum = 0;
        for (int rating : dish.second) {
            sum += rating;
        }
        double average = sum / dish.second.size();
        averageRatings.push_back({ dish.first, average });
    }

    // ��ƽ�����ֽ�������
    sort(averageRatings.begin(), averageRatings.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return b.second < a.second;
        });

    // �Ƽ�������ߵĲ�Ʒ
    string recommendedDish = averageRatings.front().first;
    cout << "�Ƽ�������ߵĲ�Ʒ:" << endl;
    search_dish(recommendedDish);
}

int main() {
    map<string, User> users = load_users("users.txt");
    map<string, Admin> admins = load_admins("admin.txt");
    User loggedInUser;
    Admin loggedInAdmin;
    int choice;
    cout << "��ӭ����ɽ��ʳ�ù���ϵͳ!" << endl;
    while (true) {
        cout << "���˵�:" << endl;
        cout << "1. �û���¼" << endl;
        cout << "2. ����Ա��¼" << endl;
        cout << "3. ������в�Ʒ" << endl;
        cout << "4. ������Ʒ" << endl;
        cout << "5. �ύ����" << endl;
        cout << "6. �Ƽ���Ʒ" << endl; // ����ѡ��
        cout << "7. �˳�" << endl;
        cout << "������ѡ��: ";
        cin >> choice;
        switch (choice) {
        case 1:
            if (user_login(users, loggedInUser)) {
                int userChoice;
                while (true) {
                    cout << "�û��˵�:" << endl;
                    cout << "1. ������в�Ʒ" << endl;
                    cout << "2. ����鿴��Ʒ" << endl;
                    cout << "3. ������Ʒ" << endl;
                    cout << "4. �ύ����" << endl;
                    cout << "5. �Ƽ���Ʒ" << endl; // ����ѡ��
                    cout << "6. �˳���¼" << endl;
                    cout << "������ѡ��: ";
                    cin >> userChoice;
                    if (userChoice == 1) {
                        view_all_dishes("ȫ��");
                    }
                    else if (userChoice == 2) {
                        cout << "��������� (����/��ʳ/����): ";
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
                        recommend_dish(); // �����º���
                    }
                    else if (userChoice == 6) {
                        break;
                    }
                    else {
                        cout << "��Чѡ��" << endl;
                    }
                }
            }
            break;
        case 2:
            if (admin_login(admins, loggedInAdmin)) {
                int adminChoice;
                while (true) {
                    cout << "����Ա�˵�:" << endl;
                    cout << "1. ����¹���Ա" << endl;
                    cout << "2. ɾ������Ա" << endl;
                    cout << "3. �޸Ĺ���Ա����" << endl;
                    cout << "4. ����²�ɫ" << endl;
                    cout << "5. ɾ����ɫ" << endl;
                    cout << "6. ����û�����" << endl;
                    cout << "7. �˳���¼" << endl;
                    cout << "������ѡ��: ";
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
                        cout << "��Чѡ��" << endl;
                    }
                }
            }
            break;
        case 3:
            view_all_dishes("ȫ��");
            break;
        case 4:
            search_dish();
            break;
        case 5:
            submit_feedback();
            break;
        case 6:
            recommend_dish(); // �����º���
            break;
        case 7:
            cout << "�˳�ϵͳ �ټ�!" << endl;
            return 0;
        default:
            cout << "��Чѡ��" << endl;
        }
    }
    return 0;
}
