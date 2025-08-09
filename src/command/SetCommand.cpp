#include "command/SetCommand.h"

SetCommand::SetCommand(std::string key, std::string value) : _key(std::move(key)), _value(std::move(value))
{
}

Response SetCommand::execute(IDatabase &db)
{
  db.set(_key, _value);
  return Response::nil();
}