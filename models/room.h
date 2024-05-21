#pragma once

#include "../queries.h"
#include "../validation.h"
#include <cstdint>
#include <iostream>
#include <istream>
#include <limits>
#include <string>

class Room {
  protected:
    uint64_t passport_number;
    uint16_t room_number;
    std::string check_in_date, check_out_date;

  public:
    Room();
    uint64_t getPassportNumber();
    std::string getInsertQuery();
    friend std::istream& operator>>(std::istream& is, Room& room);
};
