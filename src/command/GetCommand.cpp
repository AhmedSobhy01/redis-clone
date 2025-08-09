#include "command/GetCommand.h"

GetCommand::GetCommand(std::string key) : _key(std::move(key))
{
}

Response GetCommand::execute(IDatabase &db)
{
  std::string value;
  if (db.get(_key, value))
    return Response::str(value);

  return Response::nil();
}