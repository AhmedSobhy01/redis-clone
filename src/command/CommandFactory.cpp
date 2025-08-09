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

  if (cmd == "get" && req.args.size() == 2)
    return std::make_unique<GetCommand>();
  else if (cmd == "set" && req.args.size() == 3)
    return std::make_unique<SetCommand>();
  else if (cmd == "del" && req.args.size() == 2)
    return std::make_unique<DelCommand>();
  else if (cmd == "keys" && req.args.size() == 1)
    return std::make_unique<KeysCommand>();

  return nullptr;
}
