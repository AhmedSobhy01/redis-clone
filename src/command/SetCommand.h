#pragma once

#include "command/Command.h"

#include <string>
#include <vector>

class SetCommand : public Command
{
public:
  Response execute(IDatabase *db, const std::vector<std::string> &args) override;
};