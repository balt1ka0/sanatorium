#pragma once

#include "custom_exceptions.h"
#include <cstdint>
#include <format>
#include <iostream>
#include <istream>
#include <limits>
#include <sqlite3.h>

// принимает ввод, проверяет, что паспорта нет в базе данных и возвращает номер паспорта
uint64_t getPassportNumberForCreation(sqlite3*& client, char*& error);
// принимает ввод, проверяет, что паспорт есть в базе данных и возвращает номер паспорта
uint64_t getPassportNumberForInsertion(sqlite3*& client, char*& error);
// SELECT EXISTS;
int processExistsQuery(void* data, int argc, char** field_values, char** field_names);

bool checkGuest(sqlite3*& client, char*& error, uint64_t passport_number);
bool isValidDate(uint16_t day, uint16_t month, uint16_t year);
