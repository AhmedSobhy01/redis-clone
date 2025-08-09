#include "command/CommandFactory.h"
#include "command/GetCommand.h"
#include "command/SetCommand.h"
#include "command/DelCommand.h"
#include "command/KeysCommand.h"

std::unique_ptr<Command> CommandFactory::create(const Request &req)
{
  if (req.args.empty())
    return nullptr;

  const std::string &cmd = req.args[0];

  if (cmd == "get")
    return std::make_unique<GetCommand>();
  else if (cmd == "set")
    return std::make_unique<SetCommand>();
  else if (cmd == "del")
    return std::make_unique<DelCommand>();
  else if (cmd == "keys")
    return std::make_unique<KeysCommand>();

  return nullptr;
}
