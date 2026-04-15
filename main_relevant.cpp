/**
 * @file main.cpp
 * @brief Программа для сравнения алгоритмов сортировки массива объектов Train
 * @author Максим
 * @date 2026
 * 
 * Программа реализует три алгоритма сортировки (вставками, быстрая, слиянием)
 * для массива данных о расписании поездов. Выполняется бенчмарк и сравнение
 * с std::sort.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>

/**
 * @class Train
 * @brief Класс, представляющий данные о поезде
 * 
 * Содержит информацию о номере поезда, дате и времени отправления,
 * типе поезда и времени в пути. Реализует операторы сравнения для сортировки.
 */
class Train {
public:
    int trainNumber;           ///< Номер поезда
    std::string departureDate; ///< Дата отправления (формат YYYY-MM-DD)
    std::string trainType;     ///< Тип поезда (скорый, пассажирский, товарный)
    std::string departureTime; ///< Время отправления (формат HH:MM)
    int travelTimeMinutes;     ///< Время в пути в минутах

    /**
     * @brief Конструктор по умолчанию
     */
    Train() : trainNumber(0), travelTimeMinutes(0) {}

    /**
     * @brief Параметризованный конструктор
     * @param number Номер поезда
     * @param date Дата отправления
     * @param type Тип поезда
     * @param time Время отправления
     * @param travel Время в пути (минуты)
     */
    Train(int number, const std::string& date, const std::string& type,
          const std::string& time, int travel)
        : trainNumber(number), departureDate(date), trainType(type),
          departureTime(time), travelTimeMinutes(travel) {}

    /**
     * @brief Оператор "меньше" для сравнения поездов
     * 
     * Порядок сравнения:
     * 1. Дата отправления (по возрастанию)
     * 2. Время отправления (по возрастанию)
     * 3. Номер поезда (по возрастанию)
     * 4. Время в пути (по убыванию)
     * 
     * @param other Другой объект Train для сравнения
     * @return true если текущий объект меньше other
     */
    bool operator<(const Train& other) const {
        if (departureDate != other.departureDate)
            return departureDate < other.departureDate;
        if (departureTime != other.departureTime)
            return departureTime < other.departureTime;
        if (trainNumber != other.trainNumber)
            return trainNumber < other.trainNumber;
        return travelTimeMinutes > other.travelTimeMinutes;
    }

    /**
     * @brief Оператор "больше"
     * @param other Другой объект Train
     * @return true если текущий объект больше other
     */
    bool operator>(const Train& other) const {
        return other < *this;
    }

    /**
     * @brief Оператор "меньше или равно"
     * @param other Другой объект Train
     * @return true если текущий объект меньше или равен other
     */
    bool operator<=(const Train& other) const {
        return !(other < *this);
    }

    /**
     * @brief Оператор "больше или равно"
     * @param other Другой объект Train
     * @return true если текущий объект больше или равен other
     */
    bool operator>=(const Train& other) const {
        return !(*this < other);
    }

    /**
     * @brief Оператор вывода в поток
     * @param os Поток вывода
     * @param t Объект Train
     * @return Ссылка на поток вывода
     */
    friend std::ostream& operator<<(std::ostream& os, const Train& t) {
        os << t.trainNumber << "," << t.departureDate << "," << t.trainType << ","
           << t.departureTime << "," << t.travelTimeMinutes;
        return os;
    }
};

// ==================== АЛГОРИТМЫ СОРТИРОВКИ ====================

/**
 * @brief Сортировка простыми вставками (Insertion Sort)
 * 
 * Алгоритм со сложностью O(n²). Эффективен для небольших или
 * почти отсортированных массивов.
 * 
 * @param arr Вектор объектов Train для сортировки
 */
void insertionSort(std::vector<Train>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        Train key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

/**
 * @brief Вспомогательная функция разделения для быстрой сортировки
 * @param arr Вектор объектов Train
 * @param low Нижняя граница
 * @param high Верхняя граница
 * @return Индекс опорного элемента после разделения
 */
int partition(std::vector<Train>& arr, int low, int high) {
    Train pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

/**
 * @brief Рекурсивная функция быстрой сортировки
 * @param arr Вектор объектов Train
 * @param low Нижняя граница
 * @param high Верхняя граница
 */
void quickSortHelper(std::vector<Train>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

/**
 * @brief Быстрая сортировка (Quick Sort)
 * 
 * Алгоритм со средней сложностью O(n log n). Один из наиболее
 * эффективных алгоритмов сортировки на практике.
 * 
 * @param arr Вектор объектов Train для сортировки
 */
void quickSort(std::vector<Train>& arr) {
    if (!arr.empty()) {
        quickSortHelper(arr, 0, arr.size() - 1);
    }
}

/**
 * @brief Слияние двух отсортированных подмассивов
 * @param arr Вектор объектов Train
 * @param left Левая граница
 * @param mid Средняя точка
 * @param right Правая граница
 */
void merge(std::vector<Train>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Train> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

/**
 * @brief Рекурсивная функция сортировки слиянием
 * @param arr Вектор объектов Train
 * @param left Левая граница
 * @param right Правая граница
 */
void mergeSortHelper(std::vector<Train>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

/**
 * @brief Сортировка слиянием (Merge Sort)
 * 
 * Стабильный алгоритм со сложностью O(n log n). Требует
 * дополнительной памяти O(n).
 * 
 * @param arr Вектор объектов Train для сортировки
 */
void mergeSort(std::vector<Train>& arr) {
    if (!arr.empty()) {
        mergeSortHelper(arr, 0, arr.size() - 1);
    }
}

// ==================== РАБОТА С ФАЙЛАМИ ====================

/**
 * @brief Чтение данных о поездах из CSV файла
 * 
 * Формат CSV: train_number,departure_date,train_type,departure_time,travel_time_minutes
 * 
 * @param filename Путь к CSV файлу
 * @return Вектор объектов Train
 */
std::vector<Train> readFromCSV(const std::string& filename) {
    std::vector<Train> trains;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return trains;
    }

    std::string line;
    std::getline(file, line); // Пропускаем заголовок

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Train train;

        std::getline(ss, token, ',');
        train.trainNumber = std::stoi(token);

        std::getline(ss, train.departureDate, ',');
        std::getline(ss, train.trainType, ',');
        std::getline(ss, train.departureTime, ',');

        std::getline(ss, token, ',');
        train.travelTimeMinutes = std::stoi(token);

        trains.push_back(train);
    }

    file.close();
    return trains;
}

/**
 * @brief Запись отсортированных данных в CSV файл
 * @param trains Вектор объектов Train
 * @param filename Путь к выходному файлу
 */
void writeToCSV(const std::vector<Train>& trains, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
        return;
    }

    file << "train_number,departure_date,train_type,departure_time,travel_time_minutes\n";
    for (const auto& train : trains) {
        file << train << "\n";
    }

    file.close();
}

// ==================== БЕНЧМАРК ====================

/// Тип указателя на функцию сортировки
using SortFunction = void(*)(std::vector<Train>&);

/**
 * @brief Измерение времени выполнения сортировки
 * @param data Копия данных для сортировки
 * @param sortFunc Указатель на функцию сортировки
 * @return Время выполнения в миллисекундах
 */
double measureSortTime(std::vector<Train> data, SortFunction sortFunc) {
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(data);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

/**
 * @brief Обёртка для std::sort
 * @param arr Вектор объектов Train
 */
void stdSort(std::vector<Train>& arr) {
    std::sort(arr.begin(), arr.end());
}

/**
 * @brief Выполнение бенчмарка для одного файла данных
 * @param inputFile Путь к входному CSV файлу
 */
void runBenchmark(const std::string& inputFile) {
    std::vector<Train> originalData = readFromCSV(inputFile);

    if (originalData.empty()) {
        return;
    }

    int size = originalData.size();
    std::cout << std::setw(10) << size;

    double insertionTime = measureSortTime(originalData, insertionSort);
    std::cout << std::setw(15) << std::fixed << std::setprecision(2) << insertionTime;

    double quickTime = measureSortTime(originalData, quickSort);
    std::cout << std::setw(15) << quickTime;

    double mergeTime = measureSortTime(originalData, mergeSort);
    std::cout << std::setw(15) << mergeTime;

    double stdTime = measureSortTime(originalData, stdSort);
    std::cout << std::setw(15) << stdTime;

    std::cout << std::endl;

    // Сохраняем отсортированный результат
    std::vector<Train> sortedData = originalData;
    mergeSort(sortedData);
    std::string outputFile = "sorted_" + std::to_string(size) + ".csv";
    writeToCSV(sortedData, outputFile);
}

/**
 * @brief Главная функция программы
 * 
 * Выполняет бенчмарк всех алгоритмов сортировки на наборах данных
 * различного размера и выводит результаты в табличном виде.
 * 
 * @return Код возврата (0 при успехе)
 */
int main() {
    std::cout << "=== Бенчмарк алгоритмов сортировки ===" << std::endl;
    std::cout << std::endl;

    std::cout << std::setw(10) << "Размер"
              << std::setw(15) << "Вставки(мс)"
              << std::setw(15) << "Быстрая(мс)"
              << std::setw(15) << "Слияние(мс)"
              << std::setw(15) << "std::sort(мс)"
              << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    std::vector<int> sizes = {100, 250, 500, 750, 1000, 1500, 2000, 3000, 5000, 7500, 10000, 20000, 50000, 75000, 100000};

    for (int size : sizes) {
        std::string filename = "data_" + std::to_string(size) + ".csv";
        runBenchmark(filename);
    }

    std::cout << std::endl;
    std::cout << "Отсортированные данные сохранены в файлы sorted_*.csv" << std::endl;

    return 0;
}
