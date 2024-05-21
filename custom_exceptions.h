#pragma once

#include <exception>

class DatabaseException : public std::exception {
  protected:
    const char* message;

  public:
    DatabaseException(const char* _message) : message {_message} {}
    const char* what() const noexcept { return message; }
};