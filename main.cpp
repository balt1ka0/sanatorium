#include "custom_exceptions.h"
#include "queries.h"
#include <iostream>
#include <sqlite3.h>
#include <stdio.h>

void createTables() {
    sqlite3* client;
    char* error;

    if (sqlite3_open("database.db", &client))
        throw DatabaseException("can't open database\n");

    if (sqlite3_exec(client, create_guest_table, 0, 0, &error))
        throw DatabaseException("can't create guest table\n");
}

int main(void) {
    try {
        createTables();
    } catch (const DatabaseException& e) {
        std::cerr << e.what();
    }

    return 0;
}
