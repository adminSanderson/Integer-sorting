"""Создаёт best/avg/worst наборы для heap_sort_by_sarkisyan.cpp.

Запуск из корня проекта:
    python generate_heap_data.py

Файлы будут записаны в папку data/ рядом с проектом.
"""

from pathlib import Path
import random
from typing import Dict, List, Tuple

N = 20_000
SEED = 42


def count_heap_operations(values: List[int]) -> Tuple[int, int]:
    """Повторяет логику C++ Heap Sort и считает сравнения и обмены.

    Возвращает пару (количество сравнений, количество обменов).
    Эти числа используются только для выбора контрастных best/worst наборов.
    """
    a = values.copy()
    comparisons = 0
    swaps = 0

    def sift_down(heap_size: int, root: int) -> None:
        nonlocal comparisons, swaps

        while True:
            largest = root
            left = 2 * root + 1
            right = 2 * root + 2

            if left < heap_size:
                comparisons += 1
                if a[left] > a[largest]:
                    largest = left

            if right < heap_size:
                comparisons += 1
                if a[right] > a[largest]:
                    largest = right

            if largest == root:
                break

            a[root], a[largest] = a[largest], a[root]
            swaps += 1
            root = largest

    size = len(a)
    if size < 2:
        return comparisons, swaps

    for root in range(size // 2 - 1, -1, -1):
        sift_down(size, root)

    for end in range(size - 1, 0, -1):
        a[0], a[end] = a[end], a[0]
        swaps += 1
        sift_down(end, 0)

    return comparisons, swaps


def make_candidates(n: int, rng: random.Random) -> Dict[str, List[int]]:
    """Набор разных форм входных данных для выбора best и worst."""
    ascending = list(range(n))
    descending = list(range(n, 0, -1))
    all_equal = [7] * n

    alternating: List[int] = []
    for low in range((n + 1) // 2):
        high = n - 1 - low
        alternating.append(low)
        if high != low:
            alternating.append(high)

    half = (n + 1) // 2
    organ_pipe = list(range(half)) + list(range(n // 2 - 1, -1, -1))
    equal_blocks = [i // 8 for i in range(n)]
    sawtooth = [i % 64 for i in range(n)]

    random_values = ascending.copy()
    rng.shuffle(random_values)

    return {
        "ascending": ascending,
        "descending": descending,
        "all_equal": all_equal,
        "alternating_low_high": alternating,
        "organ_pipe": organ_pipe,
        "equal_blocks": equal_blocks,
        "sawtooth": sawtooth,
        "random": random_values,
    }


def save_values(path: Path, values: List[int]) -> None:
    path.write_text(" ".join(map(str, values)) + "\n", encoding="utf-8")


def main() -> None:
    project_dir = Path(__file__).resolve().parent
    data_dir = project_dir / "data"
    data_dir.mkdir(parents=True, exist_ok=True)

    rng = random.Random(SEED)
    candidates = make_candidates(N, rng)

    # Оцениваем трудоёмкость каждого кандидата теми же сравнениями и
    # обменами, которые выполняет C++-реализация.
    measured = {
        name: count_heap_operations(values)
        for name, values in candidates.items()
    }

    best_name = min(
        measured,
        key=lambda name: (sum(measured[name]), measured[name][0], measured[name][1]),
    )
    worst_name = max(
        measured,
        key=lambda name: (sum(measured[name]), measured[name][0], measured[name][1]),
    )

    # avg — отдельная случайная перестановка с фиксированным seed.
    avg_values = list(range(N))
    rng.shuffle(avg_values)

    save_values(data_dir / "heap_sort_sarkisyan_best.txt", candidates[best_name])
    save_values(data_dir / "heap_sort_sarkisyan_avg.txt", avg_values)
    save_values(data_dir / "heap_sort_sarkisyan_worst.txt", candidates[worst_name])

    print(f"Созданы 3 файла по {N:,} чисел в папке: {data_dir}")
    print("best:", best_name, "— comparisons/swaps:", measured[best_name])
    print("worst:", worst_name, "— comparisons/swaps:", measured[worst_name])
    print("avg: случайная перестановка, seed =", SEED)
    print()
    print(
        "Примечание: best и worst выбраны среди перечисленных шаблонов "
        "по сумме сравнений и обменов для данной реализации. "
        "Это экспериментальные наборы, а не разные асимптотические классы: "
        "обычная оценка Heap Sort — O(n log n)."
    )


if __name__ == "__main__":
    main()
