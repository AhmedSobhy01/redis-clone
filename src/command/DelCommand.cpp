#include "command/DelCommand.h"

DelCommand::DelCommand(std::string key) : _key(std::move(key))
{
}

Response DelCommand::execute(IDatabase &db)
{
  bool deleted = db.del(_key);
  return Response::integer(deleted ? 1 : 0);
}