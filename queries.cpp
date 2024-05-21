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

void createGuest(sqlite3*& client, char*& error) {
    Guest new_guest {};
    std::cin >> new_guest;
}