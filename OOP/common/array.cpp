#include "array.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

// Пустой массив
Array::Array() : data(nullptr), capacity(0), size(0) {}

// Выделить память на n элементов, но элементы не создавать (size = 0)
Array::Array(size_t n) : capacity(n), size(0) {
    if (n > 0) {
        data = new number[capacity];
        // Память выделена, но элементы не инициализированы
    } else {
        data = nullptr;
    }
}

// Создать массив с n элементами, заполненными value
Array::Array(size_t n, number value) : capacity(n), size(n) {
    if (n > 0) {
        data = new number[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = value;
        }
    } else {
        data = nullptr;
    }
}

// Конструктор копирования
Array::Array(const Array& other) : capacity(other.capacity), size(other.size) {
    if (capacity > 0) {
        data = new number[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    } else {
        data = nullptr;
    }
}

// Деструктор
Array::~Array() {
    delete[] data;
}

// Оператор присваивания
Array& Array::operator=(const Array& other) {
    if (this != &other) {
        delete[] data;

        capacity = other.capacity;
        size = other.size;

        if (capacity > 0) {
            data = new number[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }
    return *this;
}

// Изменить capacity (выделить новую память)
void Array::reserve(size_t newCapacity) {
    if (newCapacity < size) {
        throw std::runtime_error("Cannot reduce capacity below current size");
    }

    if (newCapacity == capacity) {
        return;  // Ничего не делаем
    }

    // Выделяем новую память
    number* newData = (newCapacity > 0) ? new number[newCapacity] : nullptr;

    // Копируем существующие элементы (первые size штук)
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    // Элементы от size до newCapacity-1 остаются неинициализированными

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

// Получить текущий размер
size_t Array::getSize() const {
    return size;
}

// Получить текущую вместимость
size_t Array::getCapacity() const {
    return capacity;
}

// Проверка на пустоту
bool Array::isEmpty() const {
    return size == 0;
}

// Добавить элемент в конец
void Array::pushBack(number value) {
    if (size == capacity) {
        throw std::runtime_error("Array is full, cannot push back. Use reserve() first.");
    }

    data[size++] = value;
}

// Удалить последний элемент
void Array::popBack() {
    if (size == 0) {
        throw std::runtime_error("Cannot pop from empty array");
    }

    --size;
    // Элемент остаётся в памяти, но больше не доступен через size
}

// Доступ к элементам (только в пределах size)
number& Array::operator[](size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

const number& Array::operator[](size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// Среднее арифметическое
number Array::mean() const {
    if (size == 0) return 0.0;

    number sum = 0.0;
    for (size_t i = 0; i < size; ++i) {
        sum += data[i];
    }
    return sum / static_cast<number>(size);
}

// Среднеквадратичное отклонение
number Array::stdDeviation() const {
    if (size <= 1) return 0.0;

    number avg = mean();
    number sumSq = 0.0;

    for (size_t i = 0; i < size; ++i) {
        number diff = data[i] - avg;
        sumSq += diff * diff;
    }

    // Среднее квадратов
    number meanSq = sumSq / static_cast<number>(size - 1);

    // Извлекаем квадратный корень (теперь есть функция sqrt для TComplex)
    return sqrt(meanSq);
}

// Сортировка по возрастанию
void Array::sortAscending() {
    if (size > 0) {
        std::sort(data, data + size);
    }
}

// Сортировка по убыванию
void Array::sortDescending() {
    if (size > 0) {
        std::sort(data, data + size, std::greater<number>());
    }
}

// Ввод из потока
// Формат: сначала размер, потом элементы (устанавливается size = n, capacity = n)
std::istream& operator>>(std::istream& in, Array& arr) {
    size_t n;
    in >> n;

    // Создаём временный массив
    number* newData = (n > 0) ? new number[n] : nullptr;

    // Читаем элементы
    for (size_t i = 0; i < n; ++i) {
        in >> newData[i];
    }

    // Заменяем старый массив
    delete[] arr.data;
    arr.data = newData;
    arr.capacity = n;
    arr.size = n;

    return in;
}

// Вывод в поток (только size элементов)
std::ostream& operator<<(std::ostream& out, const Array& arr) {
    out << "[";
    for (size_t i = 0; i < arr.size; ++i) {
        if (i > 0) out << ", ";
        out << arr.data[i];
    }
    out << "]";
    return out;
}
