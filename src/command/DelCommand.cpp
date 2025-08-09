#include "command/DelCommand.h"

DelCommand::DelCommand(const std::string &key) : _key(key)
{
}

Response DelCommand::execute(IDatabase &db)
{
  bool deleted = db.del(_key);
  return Response::integer(deleted ? 1 : 0);
}