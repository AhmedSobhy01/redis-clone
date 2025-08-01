#pragma once

#include "../containers/IDatabase.h"
#include "../protocol/Response.h"

class Command
{
public:
  virtual Response execute(IDatabase &db) = 0;
};
