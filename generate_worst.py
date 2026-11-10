import os

def generate_heap_worst_case():
    # Размер массива (как в ваших предыдущих файлах)
    n = 20000
    
    # Создаем папку data, если её вдруг нет
    os.makedirs("data", exist_ok=True)
    
    # Генерируем обратно отсортированный массив: [20000, 19999, ..., 2, 1]
    # Именно он вызывает максимальное количество просеиваний (siftDown)
    worst_case = " ".join(map(str, range(n, 0, -1)))
    
    # Путь к файлу
    file_path = "data/heap_sort_sarkisyan_worst.txt"
    
    # Записываем в файл
    with open(file_path, "w", encoding="utf-8") as f:
        f.write(worst_case)
        
    print(f"Успешно! Файл {file_path} перезаписан.")
    print(f"Сгенерировано {n} элементов (по убыванию).")

if __name__ == "__main__":
    generate_heap_worst_case()