#include "custom_exceptions.h"
#include "queries.h"
#include <cstdint>
#include <iostream>
#include <limits>
#include <sqlite3.h>
#include <stdio.h>

int main(void) {
    sqlite3* client;
    char* error;

    try {
        initTables(client, error);
    } catch (const DatabaseException& e) {
        sqlite3_free(error);
        std::cerr << e.what();
        sqlite3_close(client);
        return EXIT_FAILURE;
    }

    uint16_t action;
    while (true) {
        std::cout << "Выберите действие, которое хотите совершить:\n";
        std::cout << "0. - выйти из приложения\n";
        std::cout << "1. - создать карточку гостя\n";
        std::cout << "2. - заселить гостя в комнату\n";
        std::cout << "3. - записать гостя на лечебные процедуры\n";
        std::cout << "4. - выселить гостя\n";
        std::cout << "5. - рассчитать гостя\n";

        if (!(std::cin >> action)) {
            // если пользователь ввел не число
            std::cout << "Некорректный ввод\n";
            std::cin.clear(); // удаляем код ошибки
            // игнорируем все символы до переноса строки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        try {
            switch (action) {
            case 0:
                std::cout << "Выход из приложения\n";
                return EXIT_SUCCESS;

            case 1:
                std::cout << "Создание карточки\n";
                createGuest(client, error);
                break;

            case 2:
                std::cout << "Заселение в комнату:\n";
                createRoom(client, error);
                break;

            case 3:
                std::cout << "Запись гостя на лечебные процедуры:\n";
                createService(client, error);
                break;

            case 4:
                std::cout << "Выселение гостя\n";
                checkOutGuest(client, error);
                break;

            default:
                std::cout << "Данной команды не существует\n";
                break;
            }
        } catch (const DatabaseException& e) {
            sqlite3_free(error);
            std::cerr << e.what();
            sqlite3_close(client);
            return EXIT_FAILURE;
        }
    }

    sqlite3_close(client);
    return EXIT_SUCCESS;
}
