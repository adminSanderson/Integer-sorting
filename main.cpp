// Этот файл меняет только капитан!
#include "sorts.h"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
namespace fs = filesystem;

// ---------- Данные ----------

// Один алгоритм: имя + функция
struct Algo {
    string name;
    void (*fn)(vector<int>&);
};

vector<Algo> algos = {
    {"bubble_captain", bubblesortbycaptain},
    {"tim_vorobyev", timsortbyvorobyev},
    {"heap_sarkisyan", heapsortbysarkisyan},
    {"selection_mironova", selectionsortbymironova},
};

// Один результат замера
struct Result {
    string file;
    string algorithm;
    int n;
    double ms;
    string status;  // OK или WRONG
};

// Тут храним все результаты после обучения
vector<Result> results;

// ---------- Вспомогательные функции ----------

// Читаем числа из файла в вектор
vector<int> readFile(const fs::path& path) {
    ifstream in(path);
    if (!in) {
        cerr << "Cannot open " << path << "\n";
        return {};
    }
    vector<int> v;
    int x;
    while (in >> x) v.push_back(x);
    return v;
}

// Для русского Excel нужна запятая вместо точки
string toExcel(double x) {
    ostringstream ss;
    ss << fixed << setprecision(3) << x;
    string s = ss.str();
    replace(s.begin(), s.end(), '.', ',');
    return s;
}

// ---------- Меню ----------

void printMenu() {
    cout << "\nMenu Options:\n";
    cout << "1. Start training\n";
    cout << "2. Save data in .txt\n";
    cout << "3. Save data in .excel\n";
    cout << "4. Exit\n";
}

void startTraining() {
    cout << "Starting training...\n";
    results.clear();

    error_code ec;
    if (!fs::is_directory("data", ec)) {
        cout << "No data directory found. Create data/ with .txt files first.\n";
        return;
    }

    // Собираем все .txt файлы и сортируем имена,
    // чтобы порядок вывода был одинаковым при каждом запуске
    vector<fs::path> files;
    fs::directory_iterator it("data", ec), end;
    for (; !ec && it != end; it.increment(ec)) {
        if (it->path().extension() == ".txt") files.push_back(it->path());
    }
    if (ec) {
        cout << "Cannot read data directory: " << ec.message() << "\n";
        return;
    }
    sort(files.begin(), files.end());
    if (files.empty()) {
        cout << "No .txt files in data/.\n";
        return;
    }

    // Идём по всем .txt файлам в папке data
    for (auto& path : files) {
        string fileName = path.filename().string();
        vector<int> original = readFile(path);

        // Правильный ответ, чтобы проверять алгоритмы
        vector<int> correct = original;
        sort(correct.begin(), correct.end());

        cout << "\n" << fileName << " (n=" << original.size() << ")\n";

        // Запускаем каждый алгоритм
        for (auto& algo : algos) {
            vector<int> v = original;  // свежая копия для каждого

            auto start = chrono::steady_clock::now();
            algo.fn(v);
            auto end = chrono::steady_clock::now();

            double ms = chrono::duration<double, milli>(end - start).count();
            string status = (v == correct) ? "OK" : "WRONG";

            cout << "  " << algo.name << ": " << ms << " ms, " << status << "\n";
            results.push_back({fileName, algo.name, (int)original.size(), ms, status});
        }
    }
    cout << "\nTraining finished.\n";
}

void saveDataTxt() {
    if (results.empty()) {
        cout << "No data. Run training first (option 1).\n";
        return;
    }
    cout << "Saving data in .txt format...\n";

    ofstream out("results.txt");
    for (auto& r : results) {
        out << r.file << " | " << r.algorithm << " | n=" << r.n
            << " | " << r.ms << " ms | " << r.status << "\n";
    }
    cout << "Saved to results.txt\n";
}

void saveDataExcel() {
    if (results.empty()) {
        cout << "No data. Run training first (option 1).\n";
        return;
    }
    cout << "Saving data in .excel format...\n";

    ofstream out("results.csv");
    out << "\xEF\xBB\xBF";  // чтобы Excel понял кодировку
    out << "file;algorithm;n;time_ms;status\n";
    for (auto& r : results) {
        out << r.file << ";" << r.algorithm << ";" << r.n << ";"
            << toExcel(r.ms) << ";" << r.status << "\n";
    }
    cout << "Saved to results.csv (open it in Excel)\n";
}

// ---------- main ----------

int main() {
    int choice;

    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            if (cin.eof()) {
                cout << "\nExiting...\n";
                return 0;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            startTraining();
        } else if (choice == 2) {
            saveDataTxt();
        } else if (choice == 3) {
            saveDataExcel();
        } else if (choice == 4) {
            cout << "Exiting...\n";
            return 0;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
