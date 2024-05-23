#pragma once

#include "../validation.h"
#include <cstdint>
#include <format>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <string>

class Service {
  protected:
    uint64_t passport_number;
    std::string service_name;
    uint32_t cost;
    std::string date;

  public:
    Service();
    Service(char* service_name, uint32_t cost, char* date);
    uint64_t getPassportNumber();
    std::string getInsertQuery() const;
    uint32_t getCost() const;
    void setPassportNumber(uint64_t passport_number);
    friend std::istream& operator>>(std::istream& is, Service& service);
    friend std::ostream& operator<<(std::ostream& os, const Service& service);
};
