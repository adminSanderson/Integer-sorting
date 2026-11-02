#include "sorts.h"
#include <utility>   // swap
using namespace std;

// Сортировка выбором по возрастанию
void selectionsortbyanna(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minIndex]) minIndex = j;
        }
        if (minIndex != i) swap(a[i], a[minIndex]);
    }
}