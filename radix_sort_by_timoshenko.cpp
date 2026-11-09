#include "sorts.h"
#include <cstdint>
#include <vector>

using namespace std;

// LSD-поразрядная сортировка (от младшего байта к старшему).
// Работает с неотрицательными ключами, поэтому отрицательные числа
// предварительно сдвигаются в главной функции.
static void radixSortUnsigned(vector<uint32_t>& keys) {
    const int BITS_PER_PASS = 8;              // разбираем по одному байту
    const int BASE = 1 << BITS_PER_PASS;      // 256 возможных значений байта
    size_t n = keys.size();

    if (n < 2) {
        return;
    }

    // Ищем максимальный ключ, чтобы не делать лишних проходов.
    uint32_t maxKey = keys[0];
    for (size_t i = 1; i < n; ++i) {
        if (keys[i] > maxKey) maxKey = keys[i];
    }

    vector<uint32_t> buffer(n);
    vector<int> count(BASE);

    // Каждый проход сортирует числа по очередному байту.
    for (int shift = 0; shift < 32 && (maxKey >> shift) != 0;
         shift += BITS_PER_PASS) {
        // 1) Считаем, сколько раз встречается каждое значение байта.
        count.assign(BASE, 0);
        for (size_t i = 0; i < n; ++i) {
            int digit = (keys[i] >> shift) & (BASE - 1);
            ++count[digit];
        }

        // 2) Префиксные суммы дают границы групп в выходном массиве.
        for (int digit = 1; digit < BASE; ++digit) {
            count[digit] += count[digit - 1];
        }

        // 3) Раскладываем числа по группам с конца массива,
        //    чтобы сохранить устойчивость (stable).
        for (size_t i = n; i-- > 0;) {
            int digit = (keys[i] >> shift) & (BASE - 1);
            buffer[--count[digit]] = keys[i];
        }

        keys.swap(buffer);
    }
}

// Поразрядная сортировка целых чисел, включая отрицательные.
void radixsortbytimoshenko(vector<int>& a) {
    size_t n = a.size();
    if (n < 2) {
        return;
    }

    // Сдвигаем все числа так, чтобы минимум стал нулём.
    // Порядок чисел при этом не меняется, а ключи становятся
    // неотрицательными — их уже можно сортировать по разрядам.
    int minValue = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < minValue) minValue = a[i];
    }

    vector<uint32_t> keys(n);
    for (size_t i = 0; i < n; ++i) {
        keys[i] = static_cast<uint32_t>(
            static_cast<long long>(a[i]) - static_cast<long long>(minValue));
    }

    radixSortUnsigned(keys);

    // Возвращаем числа обратно после сдвига.
    for (size_t i = 0; i < n; ++i) {
        a[i] = static_cast<int>(
            static_cast<long long>(keys[i]) + static_cast<long long>(minValue));
    }
}
