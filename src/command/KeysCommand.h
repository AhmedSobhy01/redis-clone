#pragma once

#include "command/Command.h"

#include <vector>
#include <string>

class KeysCommand : public Command
{
public:
  Response execute(IDatabase *db, const std::vector<std::string> &args) override;
};