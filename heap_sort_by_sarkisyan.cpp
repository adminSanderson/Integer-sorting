#include "sorts.h"
#include <utility>
#include <vector>

using namespace std;

// Восстанавливает свойства максимальной кучи,
// начиная с узла root и не выходя за heapSize элементов.
static void siftDownHeapSortBySarkisyan(vector<int>& a, int heapSize, int root) {
    while (true) {
        int largest = root;
        int leftChild = 2 * root + 1;
        int rightChild = 2 * root + 2;

        // Ищем наибольший элемент среди родителя и его детей.
        if (leftChild < heapSize && a[leftChild] > a[largest]) {
            largest = leftChild;
        }

        if (rightChild < heapSize && a[rightChild] > a[largest]) {
            largest = rightChild;
        }

        // Если родитель уже больше детей, куча в этой ветви готова.
        if (largest == root) {
            break;
        }

        swap(a[root], a[largest]);
        root = largest;
    }
}

// Сортировка пирамидой по возрастанию.
void heapsortbysarkisyan(vector<int>& a) {
    int n = static_cast<int>(a.size());

    // Пустой массив и массив из одного элемента уже отсортированы.
    if (n < 2) {
        return;
    }

    // Строим максимальную кучу: наибольший элемент оказывается в a[0].
    for (int i = n / 2 - 1; i >= 0; --i) {
        siftDownHeapSortBySarkisyan(a, n, i);
    }

    // Переносим максимум в конец и восстанавливаем кучу для оставшейся части.
    for (int end = n - 1; end > 0; --end) {
        swap(a[0], a[end]);
        siftDownHeapSortBySarkisyan(a, end, 0);
    }
}
