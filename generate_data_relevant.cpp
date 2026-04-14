/**
 * @file generate_data.cpp
 * @brief Генератор тестовых данных для бенчмарка сортировок
 * @author Максим
 * @date 2025
 * 
 * Создаёт CSV файлы с случайными данными о расписании поездов
 * различного размера для тестирования алгоритмов сортировки.
 */

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

/**
 * @class TrainDataGenerator
 * @brief Генератор случайных данных о поездах
 * 
 * Создаёт CSV файлы с тестовыми данными, содержащими случайные
 * номера поездов, даты, времена и типы поездов.
 */
class TrainDataGenerator {
private:
    std::mt19937 rng;  ///< Генератор случайных чисел Mersenne Twister
    
    std::uniform_int_distribution<int> trainNumberDist{1, 9999};    ///< Распределение номеров поездов
    std::uniform_int_distribution<int> yearDist{2024, 2025};        ///< Распределение годов
    std::uniform_int_distribution<int> monthDist{1, 12};            ///< Распределение месяцев
    std::uniform_int_distribution<int> dayDist{1, 28};              ///< Распределение дней
    std::uniform_int_distribution<int> hourDist{0, 23};             ///< Распределение часов
    std::uniform_int_distribution<int> minuteDist{0, 59};           ///< Распределение минут
    std::uniform_int_distribution<int> travelTimeDist{30, 1440};    ///< Время в пути (30 мин - 24 часа)
    std::uniform_int_distribution<int> typeDist{0, 2};              ///< Тип поезда (0-2)

    /// Возможные типы поездов
    std::vector<std::string> trainTypes = {"скорый", "пассажирский", "товарный"};

public:
    /**
     * @brief Конструктор, инициализирующий генератор случайных чисел
     */
    TrainDataGenerator() : rng(std::random_device{}()) {}

    /**
     * @brief Генерация случайной даты
     * @return Строка с датой в формате YYYY-MM-DD
     */
    std::string generateDate() {
        std::ostringstream oss;
        oss << yearDist(rng) << "-"
            << std::setfill('0') << std::setw(2) << monthDist(rng) << "-"
            << std::setfill('0') << std::setw(2) << dayDist(rng);
        return oss.str();
    }

    /**
     * @brief Генерация случайного времени
     * @return Строка с временем в формате HH:MM
     */
    std::string generateTime() {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hourDist(rng) << ":"
            << std::setfill('0') << std::setw(2) << minuteDist(rng);
        return oss.str();
    }

    /**
     * @brief Генерация CSV файла с тестовыми данными
     * @param filename Имя выходного файла
     * @param count Количество записей для генерации
     */
    void generateFile(const std::string& filename, int count) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
            return;
        }

        // Заголовок CSV
        file << "train_number,departure_date,train_type,departure_time,travel_time_minutes\n";

        for (int i = 0; i < count; ++i) {
            file << trainNumberDist(rng) << ","
                 << generateDate() << ","
                 << trainTypes[typeDist(rng)] << ","
                 << generateTime() << ","
                 << travelTimeDist(rng) << "\n";
        }

        file.close();
        std::cout << "Создан файл: " << filename << " (" << count << " записей)" << std::endl;
    }
};

/**
 * @brief Главная функция генератора данных
 * 
 * Создаёт набор CSV файлов с тестовыми данными размером
 * от 100 до 100000 записей.
 * 
 * @return Код возврата (0 при успехе)
 */
int main() {
    TrainDataGenerator generator;

    /// Размеры тестовых наборов данных
    std::vector<int> sizes = {100, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000};

    std::cout << "Генерация тестовых данных...\n" << std::endl;

    for (int size : sizes) {
        std::string filename = "data_" + std::to_string(size) + ".csv";
        generator.generateFile(filename, size);
    }

    std::cout << "\nГотово! Создано " << sizes.size() << " файлов." << std::endl;

    return 0;
}
