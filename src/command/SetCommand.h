#pragma once

#include "command/Command.h"
#include <string>

class SetCommand : public Command
{
public:
  SetCommand(std::string key, std::string value);
  Response execute(IDatabase &db) override;

private:
  std::string _key;
  std::string _value;
};
