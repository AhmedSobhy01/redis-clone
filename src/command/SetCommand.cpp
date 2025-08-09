#include "command/SetCommand.h"

SetCommand::SetCommand(const std::string &key, const std::string &value) : _key(key), _value(value)
{
}

Response SetCommand::execute(IDatabase &db)
{
  db.set(_key, _value);
  return Response::nil();
}