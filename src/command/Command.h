#pragma once

#include "containers/IDatabase.h"
#include "protocol/Response.h"
#include <vector>
#include <string>

class Value;

class Command
{
public:
  virtual ~Command() = default;
  virtual Response execute(IDatabase *db, const std::vector<std::string> &args) = 0;
};
