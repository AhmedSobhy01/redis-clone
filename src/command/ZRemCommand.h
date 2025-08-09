#pragma once

#include "command/Command.h"

class ZRemCommand : public Command
{
public:
  Response execute(IDatabase *db, const std::vector<std::string> &args) override;
};
