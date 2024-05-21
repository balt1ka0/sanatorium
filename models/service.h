#pragma once

#include "../validation.h"
#include <cstdint>
#include <format>
#include <iostream>
#include <istream>
#include <limits>
#include <string>

class Service {
  protected:
    uint64_t passport_number;
    std::string service_name;
    uint16_t cost;
    std::string date;

  public:
    Service();
    uint64_t getPassportNumber();
    std::string getInsertQuery();
    friend std::istream& operator>>(std::istream& is, Service& service);
};
