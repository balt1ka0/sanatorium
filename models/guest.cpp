#include "guest.h"

Guest::Guest() : passport_number {}, last_name {}, first_name {}, patronymic {} {}

Guest::Guest(uint64_t passport_number, const std::string& last_name, const std::string& first_name,
             const std::string& patronymic)
    : passport_number {passport_number}, last_name {last_name}, first_name {first_name}, patronymic {patronymic} {}

std::istream& operator>>(std::istream& is, Guest& guest) {
    inputPassport(is, guest.passport_number);

    std::cout << "Введите фамилию:\n";
    while (true) {
        std::getline(is, guest.last_name);
        if (guest.last_name.empty() || !checkName(guest.last_name))
            std::cout << "Фамилия должна состоять только из букв и быть не пустой, повторите ввод:\n";
        else
            break;
    }

    std::cout << "Введите имя:\n";
    while (true) {
        std::getline(is, guest.first_name);
        if (guest.first_name.empty() || !checkName(guest.first_name))
            std::cout << "Имя должно состоять только из букв и быть не пустой, повторите ввод:\n";
        else
            break;
    }

    std::cout << "Введите отчество (если есть):\n";
    while (true) {
        std::getline(is, guest.patronymic);
        if (!checkName(guest.patronymic))
            std::cout << "Отчество должно состоять только из букв, повторите ввод:\n";
        else
            break;
    }

    return is;
}

std::string Guest::getInsertQuery() {
    return std::format("INSERT INTO guest VALUES ('{}', '{}', '{}', '{}');", passport_number, last_name, first_name,
                       patronymic);
}

bool checkName(const std::string& name) {
    return std::all_of(name.begin(), name.end(), [](unsigned char ch) { return (isalpha(ch) || ch == SPACE); });
}