#include "sorts.h"
#include <algorithm>   // для std::min

using namespace std;

// Слияние двух отсортированных частей a[l..m] и a[m+1..r]
static void mergeParts(vector<int>& a, int l, int m, int r) {
    vector<int> leftPart(a.begin() + l, a.begin() + m + 1);
    vector<int> rightPart(a.begin() + m + 1, a.begin() + r + 1);

    size_t i = 0, j = 0;
    int k = l;
    while (i < leftPart.size() && j < rightPart.size()) {
        if (leftPart[i] <= rightPart[j]) {
            a[k++] = leftPart[i++];
        } else {
            a[k++] = rightPart[j++];
        }
    }
    while (i < leftPart.size()) a[k++] = leftPart[i++];
    while (j < rightPart.size()) a[k++] = rightPart[j++];
}

// Сортировка вставками на отрезке a[left..right]
static void insertionSortRange(vector<int>& a, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= left && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

// Тим-сорт: блоки по RUN сортируем вставками, затем сливаем попарно с удвоением
void timsortbyvorobyev(vector<int>& a) {
    int n = static_cast<int>(a.size());
    const int RUN = 32;

    // 1) Сортируем маленькие блоки вставками
    for (int i = 0; i < n; i += RUN) {
        insertionSortRange(a, i, min(i + RUN - 1, n - 1));
    }

    // 2) Сливаем блоки, удваивая размер
    for (int size = RUN; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid   = min(left + size - 1, n - 1);
            int right = min(left + 2 * size - 1, n - 1);
            if (mid < right) {
                mergeParts(a, left, mid, right);
            }
        }
    }
}