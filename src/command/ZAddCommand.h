#pragma once

#include "command/Command.h"

class ZAddCommand : public Command
{
public:
  Response execute(IDatabase *db, const std::vector<std::string> &args) override;
};
