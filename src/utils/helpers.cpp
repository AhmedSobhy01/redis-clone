#include "helpers.h"
#include <sstream>

bool parseDouble(const std::string &str, double &result)
{
  std::istringstream iss(str);
  iss >> result;
  return !iss.fail() && iss.eof();
}

bool parseInteger(const std::string &str, int &result)
{
  std::istringstream iss(str);
  iss >> result;
  return !iss.fail() && iss.eof();
}
