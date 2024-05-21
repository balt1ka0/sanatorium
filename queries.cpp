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

bool checkGuest(sqlite3*& client, char*& error, u_int64_t passport_number) {
    std::string query {std::format("SELECT EXISTS(SELECT 1 FROM guest WHERE passport_number = {});", passport_number)};
    bool is_exists;

    if (sqlite3_exec(client, query.c_str(), proccesGuestExistsQuery, &is_exists, &error))
        throw DatabaseException("Ошибка при запросе на существование пользователя");

    return is_exists;
}

int proccesGuestExistsQuery(void* data, int argc, char** field_values, char** field_names) {
    *(bool*)data = argc && std::stoi(field_values[0]) == 1;
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

void checkOutGuest(sqlite3*& client, char*& error) {}