#pragma once

#include "command/Command.h"

class ZScoreCommand : public Command
{
public:
  Response execute(IDatabase *db, const std::vector<std::string> &args) override;
};
