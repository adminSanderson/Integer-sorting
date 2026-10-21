#include "sorts.h" // файл с объявлениями сортировок
#include <utility> // для std::swap (нужные вам библиотеки)
using namespace std; // для удобства, если не знаете, что это, то прогуглите

// Пример: пузырьковая сортировка
void bubblesortbycaptain(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;  // уже отсортировано
    }
}