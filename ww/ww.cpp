#define NOMINMAX
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <limits>
#pragma execution_character_set("utf-8")

using namespace std;

//  Структуры 
struct Group {
    string name;
};

struct Student {
    int id;
    string fullName;
    string groupName;
    vector<int> grades;
};

//  Глобальные данные 
vector<Group>   groups;
vector<Student> students;
int nextStudentId = 1;

//  Вспомогательные функции 
int readInt() {
    int x;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка ввода. Введите число: ";
    }
    return x;
}

int findGroupIndex(const string& name) {
    for (size_t i = 0; i < groups.size(); ++i)
        if (groups[i].name == name) return (int)i;
    return -1;
}

int findStudentIndex(int id) {
    for (size_t i = 0; i < students.size(); ++i)
        if (students[i].id == id) return (int)i;
    return -1;
}

double studentAverage(const Student& s) {
    if (s.grades.empty()) return 0.0;
    int sum = 0;
    for (int g : s.grades) sum += g;
    return (double)sum / s.grades.size();
}

string gradesToString(const Student& s) {
    if (s.grades.empty()) return "(нет)";
    string r;
    for (size_t i = 0; i < s.grades.size(); ++i) {
        if (i) r += " ";
        r += to_string(s.grades[i]);
    }
    return r;
}

// Печать
void Students() {
    if (students.empty()) { cout << "Студентов нет.\n"; return; }
    for (const auto& s : students) {
        cout << "ID " << s.id << ": " << s.fullName
            << " | Группа: " << s.groupName
            << " | Оценки: " << gradesToString(s);
        if (!s.grades.empty())
            cout << " | Средний балл: " << fixed << setprecision(2)
            << studentAverage(s);
        cout << "\n";
    }
}

void Groups() {
    if (groups.empty()) { cout << "Групп нет.\n"; return; }
    for (size_t i = 0; i < groups.size(); ++i) {
        int count = 0;
        for (const auto& s : students)
            if (s.groupName == groups[i].name) ++count;
        cout << i + 1 << ". " << groups[i].name
            << " (студентов: " << count << ")\n";
    }
}

//  студенты
void addStudent() {
    if (groups.empty()) { cout << "Сначала создайте хотя бы одну группу.\n"; return; }
    cout << "Доступные группы:\n";
    Groups();
    cout << "Выберите номер группы: ";
    int idx = readInt();
    if (idx < 1 || idx >(int)groups.size()) { cout << "Неверный номер.\n"; return; }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Введите ФИО студента: ";
    string fio; getline(cin, fio);
    if (fio.empty()) { cout << "ФИО пустое.\n"; return; }

    Student s;
    s.id = nextStudentId++;
    s.fullName = fio;
    s.groupName = groups[idx - 1].name;
    students.push_back(s);
    cout << "Студент добавлен. ID = " << s.id << "\n";
}

void editStudent() {
    if (students.empty()) { cout << "Студентов нет.\n"; return; }
    Students();
    cout << "Введите ID студента: ";
    int id = readInt();
    int i = findStudentIndex(id);
    if (i == -1) { cout << "Студент не найден.\n"; return; }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Новое ФИО (Enter — оставить без изменений): ";
    string fio; getline(cin, fio);
    if (!fio.empty()) students[i].fullName = fio;

    if (!groups.empty()) {
        Groups();
        cout << "Номер новой группы (0 — оставить): ";
        int g = readInt();
        if (g >= 1 && g <= (int)groups.size())
            students[i].groupName = groups[g - 1].name;
    }
    cout << "Данные студента обновлены.\n";
}

void deleteStudent() {
    if (students.empty()) { cout << "Студентов нет.\n"; return; }
    Students();
    cout << "Введите ID для удаления: ";
    int id = readInt();
    int i = findStudentIndex(id);
    if (i == -1) { cout << "Студент не найден.\n"; return; }
    students.erase(students.begin() + i);
    cout << "Студент удалён.\n";
}

//  группы
void addGroup() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Введите название новой группы: ";
    string name; getline(cin, name);
    if (name.empty()) { cout << "Название пустое.\n"; return; }
    if (findGroupIndex(name) != -1) { cout << "Такая группа уже существует.\n"; return; }
    groups.push_back({ name });
    cout << "Группа \"" << name << "\" добавлена.\n";
}

void renameGroup() {
    if (groups.empty()) { cout << "Групп нет.\n"; return; }
    Groups();
    cout << "Введите номер группы для переименования: ";
    int idx = readInt();
    if (idx < 1 || idx >(int)groups.size()) { cout << "Неверный номер.\n"; return; }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Новое название: ";
    string newName; getline(cin, newName);
    if (newName.empty()) { cout << "Название пустое.\n"; return; }
    if (findGroupIndex(newName) != -1 && newName != groups[idx - 1].name) {
        cout << "Группа с таким названием уже существует.\n"; return;
    }

    string oldName = groups[idx - 1].name;
    groups[idx - 1].name = newName;
    for (auto& s : students)
        if (s.groupName == oldName) s.groupName = newName;
    cout << "Группа переименована.\n";
}

void deleteGroup() {
    if (groups.empty()) { cout << "Групп нет.\n"; return; }
    Groups();
    cout << "Введите номер группы для удаления: ";
    int idx = readInt();
    if (idx < 1 || idx >(int)groups.size()) { cout << "Неверный номер.\n"; return; }

    string name = groups[idx - 1].name;
    int count = 0;
    for (const auto& s : students) if (s.groupName == name) ++count;

    if (count > 0) {
        cout << "Нельзя удалить группу: в ней " << count << " студент(ов).\n";
        return;
    }
    groups.erase(groups.begin() + idx - 1);
    cout << "Группа удалена.\n";
}

//  Оценки 
void addGrade() {
    if (students.empty()) { cout << "Студентов нет.\n"; return; }
    Students();
    cout << "Введите ID студента: ";
    int id = readInt();
    int i = findStudentIndex(id);
    if (i == -1) { cout << "Студент не найден.\n"; return; }

    cout << "Сколько оценок добавить? ";
    int n = readInt();
    if (n <= 0) return;

    for (int k = 0; k < n; ++k) {
        cout << "Оценка " << k + 1 << " (2..5): ";
        int g = readInt();
        if (g < 2 || g > 5) { cout << "Недопустимая оценка, пропущена.\n"; continue; }
        students[i].grades.push_back(g);
    }
    cout << "Оценки добавлены.\n";
}

//  Средние баллы 
void showStudentAverage() {
    if (students.empty()) { cout << "Студентов нет.\n"; return; }
    Students();
    cout << "Введите ID студента: ";
    int id = readInt();
    int i = findStudentIndex(id);
    if (i == -1) { cout << "Студент не найден.\n"; return; }
    if (students[i].grades.empty()) {
        cout << "У студента " << students[i].fullName << " нет оценок.\n"; return;
    }
    cout << "Средний балл студента " << students[i].fullName << ": "
        << fixed << setprecision(2) << studentAverage(students[i]) << "\n";
}

void showGroupAverage() {
    if (groups.empty()) { cout << "Групп нет.\n"; return; }
    Groups();
    cout << "Введите номер группы: ";
    int idx = readInt();
    if (idx < 1 || idx >(int)groups.size()) { cout << "Неверный номер.\n"; return; }

    string name = groups[idx - 1].name;
    double sum = 0; int cnt = 0;
    for (const auto& s : students) {
        if (s.groupName != name) continue;
        for (int g : s.grades) { sum += g; ++cnt; }
    }
    if (cnt == 0) { cout << "В группе нет оценок.\n"; return; }
    cout << "Средний балл по группе " << name << ": "
        << fixed << setprecision(2) << (sum / cnt) << "\n";
}

//  Статистика группы 
void showGroupStatistics() {
    if (groups.empty()) { cout << "Групп нет.\n"; return; }
    Groups();
    cout << "Введите номер группы: ";
    int idx = readInt();
    if (idx < 1 || idx >(int)groups.size()) { cout << "Неверный номер.\n"; return; }

    string name = groups[idx - 1].name;
    int total = 0, otl = 0, hor = 0, tro = 0, dvo = 0, noG = 0;

    for (const auto& s : students) {
        if (s.groupName != name) continue;
        ++total;
        if (s.grades.empty()) { ++noG; continue; }
        double avg = studentAverage(s);
        if (avg >= 4.5) ++otl;
        else if (avg >= 3.5) ++hor;
        else if (avg >= 2.5) ++tro;
        else                 ++dvo;
    }

    cout << "\n=== Статистика группы " << name << " ===\n";
    cout << "Всего студентов : " << total << "\n";
    cout << "Отличников      : " << otl << "\n";
    cout << "Хорошистов      : " << hor << "\n";
    cout << "Троечников      : " << tro << "\n";
    cout << "Двоечников      : " << dvo << "\n";
    if (noG > 0) cout << "Без оценок      : " << noG << "\n";
}

//  Начальные данные 
void initData() {
    groups.push_back({ "БД-1-25" });
    groups.push_back({ "ИСиП-13-25" });

    students.push_back({ nextStudentId++, "Иванов Иван Иванович",       "ИСиП-13-25", {5,5,5,4,5} });
    students.push_back({ nextStudentId++, "Зопунян Виталий Зорикович",       "БД-1-25", {2,4,5,3,4} });
    students.push_back({ nextStudentId++, "Тимофеев Артём Александрович",  "БД-1-25", {4,5,4,5,3} });
    students.push_back({ nextStudentId++, "Оганнисян Мушег Григорович",    "ИСиП-13-25", {2,3,3,2,3} });
    students.push_back({ nextStudentId++, "Максимкин Федор Алексеевич",   "БД-1-25", {5,5,5,5,5} });
    students.push_back({ nextStudentId++, "Гулиев Туран Фагани Оглы",   "ИСиП-13-25", {2,3,2,3,3} });
}

// ---------- Меню ----------
void MainMenu() {
    cout << "\nУправление учебными группами\n"
        << " 1. Показать всех студентов\n"
        << " 2. Показать все группы\n"
        << " 3. Добавить студента\n"
        << " 4. Изменить студента\n"
        << " 5. Удалить студента\n"
        << " 6. Добавить группу\n"
        << " 7. Переименовать группу\n"
        << " 8. Удалить группу\n"
        << " 9. Добавить оценки студенту\n"
        << "10. Средний балл студента\n"
        << "11. Средний балл группы\n"
        << "12. Статистика группы\n"
        << " 0. Выход\n"
        << "Выбор: ";
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    initData();

    int choice;
    do {
        MainMenu();
        choice = readInt();
        cout << "\n";
        switch (choice) {
        case 1:  Students();       break;
        case 2:  Groups();         break;
        case 3:  addStudent();          break;
        case 4:  editStudent();         break;
        case 5:  deleteStudent();       break;
        case 6:  addGroup();            break;
        case 7:  renameGroup();         break;
        case 8:  deleteGroup();         break;
        case 9:  addGrade();            break;
        case 10: showStudentAverage();  break;
        case 11: showGroupAverage();    break;
        case 12: showGroupStatistics(); break;
        case 0:  cout << "Выход из программы.\n"; break;
        default: cout << "Неверный пункт меню.\n"; break;
        }
    } while (choice != 0);

    return 0;
}