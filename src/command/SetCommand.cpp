#include "command/SetCommand.h"

#include <memory>

SetCommand::SetCommand(std::string key, std::string value) : _key(std::move(key)), _value(std::move(value))
{
}

Response SetCommand::execute(IDatabase &db)
{
  Response resp;
  db.set(_key, _value);
  resp.status = ResponseStatus::OK;
  return resp;
}
