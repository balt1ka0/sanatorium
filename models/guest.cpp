#include "guest.h"

Guest::Guest() : passport_number {}, last_name {}, first_name {}, patronymic {} {}

Guest::Guest(uint64_t passport_number, const std::string& last_name, const std::string& first_name,
             const std::string& patronymic)
    : passport_number {passport_number}, last_name {last_name}, first_name {first_name}, patronymic {patronymic} {}

void Guest::GetDataFromQuery(char** data) {
    first_name = data[0];
    last_name = data[1];
    patronymic = data[2];
}

bool Guest::isInitialized() const { return !first_name.empty(); }

void Guest::setPassportNumber(uint64_t passport_number) { this->passport_number = passport_number; }

std::istream& operator>>(std::istream& is, Guest& guest) {
    std::cout << "Введите фамилию:\n";
    while (true) {
        std::getline(is, guest.last_name);
        if (guest.last_name.empty())
            std::cout << "Фамилия должна состоять только из букв и быть не пустой, повторите ввод:\n";
        else
            break;
    }

    std::cout << "Введите имя:\n";
    while (true) {
        std::getline(is, guest.first_name);
        if (guest.first_name.empty())
            std::cout << "Имя должно состоять только из букв и быть не пустой, повторите ввод:\n";
        else
            break;
    }

    std::cout << "Введите отчество (если есть):\n";
    std::getline(is, guest.patronymic);

    return is;
}

std::ostream& operator<<(std::ostream& os, const Guest& guest) {
    os << "Данные пользователя: ";
    os << guest.last_name << ' ' << guest.first_name;
    if (!guest.patronymic.empty())
        os << ' ' << guest.patronymic;
    os << '\n';
    return os;
}

std::string Guest::getInsertQuery() const {
    return std::format("INSERT INTO guest VALUES ('{}', '{}', '{}', '{}');", passport_number, last_name, first_name,
                       patronymic);
}
