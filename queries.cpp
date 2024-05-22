#include "queries.h"

void initTables(sqlite3*& client, char*& error) {
    if (sqlite3_open("database.db", &client))
        throw DatabaseException("Невозможно открыть базу данных");

    if (sqlite3_exec(client, CreateQueries::CREATE_GUEST_TABLE, 0, 0, &error))
        throw DatabaseException("Ошибка при создании таблицы guest");

    if (sqlite3_exec(client, CreateQueries::CREATE_ROOM_TABLE, 0, 0, &error))
        throw DatabaseException("Ошибка при создании таблицы room");

    if (sqlite3_exec(client, CreateQueries::CREATE_SERVICE_TABLE, 0, 0, &error))
        throw DatabaseException("Ошибка при создании таблицы service");

    std::cout << "База данных успешно инициализирована\n";
}

// функции для создания пользователя

void createGuest(sqlite3*& client, char*& error) {
    Guest new_guest {};
    std::cin >> new_guest;
    std::string query {new_guest.getInsertQuery()};
    int32_t result {sqlite3_exec(client, query.c_str(), 0, 0, &error)};

    if (result) {
        if (result == SQLITE_CONSTRAINT) {
            std::cout << "Пользователь с таким паспортом уже существует!";
            return;
        }

        throw DatabaseException("Ошибка при создании нового пользователя");
    }

    std::cout << "Гость успешно создан.\n";
}

bool checkGuest(sqlite3*& client, char*& error, uint64_t passport_number) {
    std::string query {std::format("SELECT EXISTS(SELECT 1 FROM guest WHERE passport_number = {});", passport_number)};
    bool is_exists;

    if (sqlite3_exec(client, query.c_str(), processExistsQuery, &is_exists, &error))
        throw DatabaseException("Ошибка при запросе на существование пользователя");

    return is_exists;
}

Guest getGuestNameFromPassport(sqlite3*& client, char*& error, uint64_t passport_number) {
    Guest guest;
    std::string query {std::format("SELECT last_name, first_name, patronymic FROM guest WHERE passport_number = {};",
                                   passport_number)};

    if (sqlite3_exec(client, query.c_str(), processGuestQuery, &guest, &error))
        throw DatabaseException("Ошибка при поиске гостя");

    return guest;
}

int processExistsQuery(void* data, int argc, char** field_values, char** field_names) {
    *(bool*)data = argc && std::stoi(field_values[0]) == 1;
    return 0;
}

int processGuestQuery(void* data, int argc, char** field_values, char** field_names) {
    if (argc) {
        ((Guest*)data)->GetDataFromQuery(field_values);
    }
    return 0;
}

/*
int proccesGuestExistsQuery(void* data, int argc, char** field_values, char** field_names) {
    for (auto i {0}; i != argc; ++i) {
        std::cout << field_values[i] << ' ' << field_names[i] << ' ';
    }
    std::cout << '\n';
    return 0;
}*/

void createRoom(sqlite3*& client, char*& error) {
    Room new_room {};
    std::cin >> new_room;

    if (!checkGuest(client, error, new_room.getPassportNumber())) {
        std::cout << "Гостя с таким паспортом не существует!\n";
        return;
    }

    if (sqlite3_exec(client, new_room.getInsertQuery().c_str(), 0, 0, &error))
        throw DatabaseException("Ошибка при заселении");

    std::cout << "Гость заселен\n";
}

void createService(sqlite3*& client, char*& error) {
    Service new_service {};
    std::cin >> new_service;

    if (!checkGuest(client, error, new_service.getPassportNumber())) {
        std::cout << "Гостя с таким паспортом не существует!\n";
        return;
    }

    if (sqlite3_exec(client, new_service.getInsertQuery().c_str(), 0, 0, &error))
        throw DatabaseException("Ошибка при записи на услугу");

    std::cout << "Гость записан\n";
}

void checkOutGuest(sqlite3*& client, char*& error) {
    uint64_t passport_number;

    std::cout << "Введите номер паспорта:\n";
    while (true) {
        if (!(std::cin >> passport_number)) {
            std::cout << "Паспорт должен состоять только из чисел, повторите ввод:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            if (!checkGuest(client, error, passport_number))
                std::cout << "Гостя не существует!\n";
            else
                break;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string check_out_date;
    std::cout << "Введите дату выезда в формате yyyy-mm-dd:\n";
    uint16_t day, month, year;
    while (true) {
        std::getline(std::cin, check_out_date);
        sscanf(check_out_date.c_str(), "%hu-%hu-%hu", &year, &month, &day);
        if (!isValidDate(day, month, year)) {
            std::cout << "Неверная дата, повторите ввод:\n";
        } else
            break;
    }

    if (!checkOutDate(client, error, passport_number, check_out_date)) {
        std::cout << "Либо гость выписан, либо дата заезда позже даты выезда\n";
        return;
    }

    std::string query {
        std::format("UPDATE room SET check_out_date = '{}' WHERE passport_number = '{}' AND check_in_date = (SELECT "
                    "MAX(check_in_date) FROM room WHERE passport_number = '{}' AND check_out_date IS NULL);",
                    check_out_date, passport_number, passport_number)};

    if (sqlite3_exec(client, query.c_str(), 0, 0, &error))
        throw DatabaseException("Неудалось выписать гостя\n");

    std::cout << "Гость выписан\n";
}

bool checkOutDate(sqlite3*& client, char*& error, uint64_t passport_number, const std::string& check_out_date) {
    std::string query {std::format("SELECT EXISTS(SELECT 1 FROM room WHERE passport_number = {} AND check_in_date < "
                                   "'{}' AND check_out_date IS NULL);",
                                   passport_number, check_out_date)};
    bool is_exists;

    if (sqlite3_exec(client, query.c_str(), processExistsQuery, &is_exists, &error))
        throw DatabaseException("Ошибка при запросе на существование комнаты");

    return is_exists;
}

void payment(sqlite3*& client, char*& error) {
    uint32_t cost_per_day {2000};
    uint64_t passport_number;
    Guest guest;

    std::cout << "Введите номер паспорта:\n";
    while (true) {
        if (!(std::cin >> passport_number)) {
            std::cout << "Паспорт должен состоять только из чисел, повторите ввод:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            guest = getGuestNameFromPassport(client, error, passport_number);
            if (!guest.isInitialized())
                std::cout << "Пользователя с таким паспортом не существует!\n";
            else
                break;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string query {std::format("SELECT check_in_date, check_out_date, JULIANDAY(check_out_date) - "
                                   "JULIANDAY(check_in_date) AS date_diff FROM room "
                                   "WHERE passport_number = '{}' AND check_out_date = (SELECT "
                                   "MAX(check_out_date) FROM room WHERE passport_number = '{}');",
                                   passport_number, passport_number)};

    Date date;
    if (sqlite3_exec(client, query.c_str(), processDateQuery, &date, &error))
        throw DatabaseException("Ошибка при подсчете количества дней");

    uint32_t total_cost {cost_per_day * date.date_diff};

    std::vector<Service> services {
        getServiceFromPeriod(client, error, passport_number, date.check_in_date, date.check_out_date)};
    std::cout << guest;
    std::cout << date;
    std::cout << "Услуги:\n";
    for (const Service& service : services) {
        std::cout << service;
        total_cost += service.getCost();
    }
    std::cout << "Конечный счет: " << total_cost << '\n';
}

int processDateQuery(void* data, int argc, char** field_values, char** field_names) {
    if (argc) {
        ((Date*)data)->check_in_date = field_values[0];
        ((Date*)data)->check_out_date = field_values[1];
        ((Date*)data)->date_diff = std::stoi(field_values[2]);
    } else
        ((Date*)data)->date_diff = 0;
    return 0;
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << "Дата заезда: " << date.check_in_date << '\n';
    if (!date.check_out_date.empty()) {
        std::cout << "Дата выезда: " << date.check_out_date << '\n';
        std::cout << "Продолжительность: " << date.date_diff << '\n';
    }
    return os;
}

std::vector<Service> getServiceFromPeriod(sqlite3*& client, char*& error, uint64_t passport_number,
                                          const std::string& check_in_date, const std::string& check_out_date) {
    std::string query {std::format(
        "SELECT service_name, cost, date FROM service WHERE passport_number = {} AND date BETWEEN '{}' AND '{}';",
        passport_number, check_in_date, check_out_date)};
    std::vector<Service> result {};

    if (sqlite3_exec(client, query.c_str(), processServiceQuery, &result, &error))
        throw DatabaseException(error);

    return result;
}

int processServiceQuery(void* data, int argc, char** field_values, char** field_names) {
    if (argc)
        ((std::vector<Service>*)data)->push_back(Service(field_values[0], std::stoi(field_values[1]), field_values[2]));
    return 0;
}

void getGuestData(sqlite3*& client, char*& error) {
    uint64_t passport_number;
    Guest guest;

    std::cout << "Введите номер паспорта:\n";
    while (true) {
        if (!(std::cin >> passport_number)) {
            std::cout << "Паспорт должен состоять только из чисел, повторите ввод:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            guest = getGuestNameFromPassport(client, error, passport_number);
            if (!guest.isInitialized())
                std::cout << "Пользователя с таким паспортом не существует!\n";
            else
                break;
        }
    }

    std::string room_query {std::format("SELECT check_in_date, check_out_date FROM room "
                                        "WHERE passport_number = {};",
                                        passport_number)};

    std::cout << guest;
    std::cout << "Дата заезда       Дата выезда\n";
    if (sqlite3_exec(
            client, room_query.c_str(),
            [](void* data, int argc, char** field_values, char** field_names) {
                if (argc == 2 && field_values[0] && field_values[1]) {
                    std::cout << std::setw(17) << std::left << field_values[0] << " " << std::setw(18) << std::left
                              << field_values[1] << '\n';
                } else
                    std::cout << field_values[0] << '\n';

                return 0;
            },
            nullptr, &error))
        throw DatabaseException(error);

    std::string service_query {
        std::format("SELECT service_name, cost, date FROM service WHERE passport_number = {};", passport_number)};

    std::vector<Service> services;

    if (sqlite3_exec(client, service_query.c_str(), processServiceQuery, &services, &error))
        throw DatabaseException(error);

    std::cout << "Услуги:\n";
    for (const Service& service : services)
        std::cout << service;
}