#pragma once

#include "command/Command.h"

class SetCommand : public Command
{
public:
  SetCommand(const std::string &key, const std::string &value);
  Response execute(IDatabase &db) override;

private:
  std::string _key;
  std::string _value;
};