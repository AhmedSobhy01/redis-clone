#pragma once

#include "command/Command.h"

class ZQueryCommand : public Command
{
public:
  Response execute(IDatabase *db, const std::vector<std::string> &args) override;
};
