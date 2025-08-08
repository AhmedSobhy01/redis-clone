#pragma once

#include "protocol/Request.h"
#include "command/Command.h"
#include <memory>

class CommandFactory
{
public:
  static std::unique_ptr<Command> create(const Request &req);
};
