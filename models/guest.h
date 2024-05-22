#pragma once

#include "../validation.h"
#include <algorithm>
#include <cstdint>
#include <format>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
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
    std::string getInsertQuery() const;
    void GetDataFromQuery(char** data);
    bool isInitialized() const;

    friend std::istream& operator>>(std::istream& is, Guest& guest);
    friend std::ostream& operator<<(std::ostream& os, const Guest& guest);
};

bool checkName(const std::string& name);