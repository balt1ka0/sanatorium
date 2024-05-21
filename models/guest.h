#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <istream>
#include <limits>
#include <string>

#define SPACE 32

class Guest {
  protected:
    uint64_t passport_number;
    std::string last_name, first_name, patronymic;

  public:
    Guest();
    Guest(uint64_t passport_number, const std::string& last_name, const std::string& first_name,
          const std::string& patronymic);

    friend std::istream& operator>>(std::istream& is, Guest& guest);
};

bool checkName(const std::string& name);