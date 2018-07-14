#ifndef ERROR_H_
#define ERROR_H_
#include <stdexcept>
#include <string>

class GateError : public std::logic_error
{
  public:
    explicit GateError(const std::string &str) : logic_error(str) {}
};

class CircuitError : public std::logic_error
{
  public:
    explicit CircuitError(const std::string &str) : logic_error(str) {}
};
#endif