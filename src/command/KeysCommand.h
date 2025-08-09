#pragma once

#include "command/Command.h"

class KeysCommand : public Command
{
public:
  KeysCommand();
  Response execute(IDatabase &db) override;
};
