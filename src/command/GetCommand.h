#pragma once

#include "Command.h"
#include <string>

class GetCommand : public Command
{
public:
  GetCommand(std::string key);
  Response execute(IDatabase &db) override;

private:
  std::string _key;
};
