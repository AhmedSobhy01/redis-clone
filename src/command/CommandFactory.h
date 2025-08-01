#pragma once

#include "../protocol/Request.h"
#include "Command.h"
#include <memory>

class CommandFactory
{
public:
  static std::unique_ptr<Command> create(const Request &req);
};
