#pragma once

#include "custom_exceptions.h"
#include "models/guest.h"
#include "models/room.h"
#include "models/service.h"
#include <cstdint>
#include <iostream>
#include <sqlite3.h>

namespace CreateQueries {

// один и тот же адрес для переменной в разных файлах
inline constexpr char CREATE_GUEST_TABLE[] {"CREATE TABLE IF NOT EXISTS guest (passport_number INT PRIMARY KEY, "
                                            "last_name TEXT NOT NULL, "
                                            "first_name TEXT NOT NULL, "
                                            "patronymic TEXT);"};

inline constexpr char CREATE_ROOM_TABLE[] {"CREATE TABLE IF NOT EXISTS room (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                           "passport_number INT, "
                                           "room_number INT, "
                                           "check_in_date DATE, "
                                           "check_out_date DATE NULL, "
                                           "FOREIGN KEY (passport_number) REFERENCES guest(passport_number));"};

inline constexpr char CREATE_SERVICE_TABLE[] {
    "CREATE TABLE IF NOT EXISTS service (id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "passport_number INT, "
    "service_name TEXT NOT NULL, "
    "cost INT NOT NULL, "
    "date DATE NOT NULL, "
    "FOREIGN KEY (passport_number) REFERENCES guest(passport_number));"};

} // namespace CreateQueries

// открывает базу данных, если ее нет, то создает ее и все нужные таблицы
void initTables(sqlite3*& client, char*& error);

// создает нового гостя в таблице guest
void createGuest(sqlite3*& client, char*& error);

// проверяет, существует ли гость в таблице guest
bool checkGuest(sqlite3*& client, char*& error, u_int64_t passport_number);

// эта функция применяется для каждого найденного ряда запроса
// SELECT EXISTS(SELECT 1 FROM guest WHERE passport_number = {});
int proccesGuestExistsQuery(void* data, int argc, char** field_values, char** field_names);

// заселяет гостя
void createRoom(sqlite3*& client, char*& error);

void createService(sqlite3*& client, char*& error);

void checkOutGuest(sqlite3*& client, char*& error);