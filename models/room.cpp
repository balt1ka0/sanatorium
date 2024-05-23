#include "room.h"

Room::Room() : passport_number {}, room_number {}, check_in_date {}, check_out_date {} {}

uint64_t Room::getPassportNumber() { return passport_number; }

std::string Room::getInsertQuery() {
    return std::format("INSERT INTO room (passport_number, room_number, check_in_date) VALUES ('{}', '{}', '{}')",
                       passport_number, room_number, check_in_date);
}

void Room::setPassportNumber(uint64_t passport_number) { this->passport_number = passport_number; }

std::istream& operator>>(std::istream& is, Room& room) {
    std::cout << "Введите номер комнаты:\n";
    while (true) {
        if (!(is >> room.room_number)) {
            std::cout << "Номер комнаты должен быть числом, повторите ввод:\n";
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else
            break;
    }
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Введите дату заезда в формате yyyy-mm-dd:\n";
    uint16_t day, month, year;
    while (true) {
        std::getline(is, room.check_in_date);
        sscanf(room.check_in_date.c_str(), "%hu-%hu-%hu", &year, &month, &day);
        if (!isValidDate(day, month, year)) {
            std::cout << "Неверная дата, повторите ввод:\n";
        } else
            break;
    }

    return is;
}
