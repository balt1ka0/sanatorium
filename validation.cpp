#include "validation.h"

uint64_t getPassportNumberForCreation(sqlite3*& client, char*& error) {
    uint64_t passport_number;
    std::cout << "Введите номер паспорта:\n";
    while (true) {
        if (!(std::cin >> passport_number)) {
            std::cout << "Паспорт должен состоять только из чисел, повторите ввод:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            if (!checkGuest(client, error, passport_number))
                break;
            else
                std::cout << "Пользователь с таким паспортом уже существует, повторите ввод:\n";
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return passport_number;
}

uint64_t getPassportNumberForInsertion(sqlite3*& client, char*& error) {
    uint64_t passport_number;
    std::cout << "Введите номер паспорта:\n";
    while (true) {
        if (!(std::cin >> passport_number)) {
            std::cout << "Паспорт должен состоять только из чисел, повторите ввод:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            if (checkGuest(client, error, passport_number))
                break;
            else
                std::cout << "Пользователь с таким паспортом не существует, повторите ввод:\n";
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return passport_number;
}

bool checkGuest(sqlite3*& client, char*& error, uint64_t passport_number) {
    std::string query {std::format("SELECT EXISTS(SELECT 1 FROM guest WHERE passport_number = {});", passport_number)};
    bool is_exists;

    if (sqlite3_exec(client, query.c_str(), processExistsQuery, &is_exists, &error))
        throw DatabaseException("Ошибка при запросе на существование пользователя");

    return is_exists;
}

int processExistsQuery(void* data, int argc, char** field_values, char** field_names) {
    *(bool*)data = argc && std::stoi(field_values[0]) == 1;
    return 0;
}

bool isValidDate(uint16_t day, uint16_t month, uint16_t year) {
    // Проверка на корректность года
    if (year < 1900 || year > 9999) {
        return false;
    }

    // Проверка на корректность месяца
    if (month < 1 || month > 12) {
        return false;
    }

    // Проверка на корректность дня
    if (day < 1 || day > 31) {
        return false;
    }

    // Дополнительная проверка для февраля (учет високосных годов)
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29) {
                return false;
            }
        } else {
            if (day > 28) {
                return false;
            }
        }
    }

    // Проверка на корректность дня для месяцев с 30 днями
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return false;
        }
    }

    return true;
}