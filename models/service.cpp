#include "service.h"

Service::Service() : passport_number {}, service_name {}, cost {}, date {} {}

uint64_t Service::getPassportNumber() { return passport_number; }

std::string Service::getInsertQuery() {
    return std::format(
        "INSERT INTO service (passport_number, service_name, cost, date) VALUES ('{}', '{}', '{}', '{}');",
        passport_number, service_name, cost, date);
}

std::istream& operator>>(std::istream& is, Service& service) {
    inputPassport(is, service.passport_number);

    std::cout << "Введите название услуги:\n";
    std::getline(is, service.service_name);

    std::cout << "Введите цену услуги:\n";
    while (true) {
        if (!(is >> service.cost)) {
            std::cout << "Цена должна состоять только из чисел, повторите ввод:\n";
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else
            break;
    }
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Введите дату заезда в формате yyyy-mm-dd:\n";
    uint16_t day, month, year;
    while (true) {
        std::getline(is, service.date);
        sscanf(service.date.c_str(), "%hu-%hu-%hu", &year, &month, &day);
        if (!isValidDate(day, month, year)) {
            std::cout << "Неверная дата, повторите ввод:\n";
        } else
            break;
    }

    return is;
}
