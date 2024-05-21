#pragma once

#include <cstdint>
#include <iostream>
#include <istream>
#include <limits>

void inputPassport(std::istream& is, uint64_t& passport_number);
bool isValidDate(uint16_t day, uint16_t month, uint16_t year);
