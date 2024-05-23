#include "service.h"

Service::Service() : passport_number {}, service_name {}, cost {}, date {} {}

Service::Service(char* service_name, uint32_t cost, char* date)
    : passport_number {}, service_name {service_name}, cost {cost}, date {date} {}

uint64_t Service::getPassportNumber() { return passport_number; }

uint32_t Service::getCost() const { return cost; }

std::string Service::getInsertQuery() const {
    return std::format(
        "INSERT INTO service (passport_number, service_name, cost, date) VALUES ('{}', '{}', '{}', '{}');",
        passport_number, service_name, cost, date);
}

void Service::setPassportNumber(uint64_t passport_number) { this->passport_number = passport_number; }

std::istream& operator>>(std::istream& is, Service& service) {
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

    std::cout << "Введите дату записи в формате yyyy-mm-dd:\n";
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

std::ostream& operator<<(std::ostream& os, const Service& service) {
    os << service.service_name << ' ' << service.date << ' ' << service.cost << '\n';
    return os;
}