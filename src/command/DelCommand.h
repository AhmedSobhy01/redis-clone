#pragma once

#include "command/Command.h"

#include <string>

class DelCommand : public Command
{
public:
  DelCommand(std::string key);
  Response execute(IDatabase &db) override;

private:
  std::string _key;
};
