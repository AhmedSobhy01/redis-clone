#pragma once

#include "command/Command.h"

#include <string>

class GetCommand : public Command
{
public:
  GetCommand(const std::string &key);
  Response execute(IDatabase &db) override;

private:
  std::string _key;
};
