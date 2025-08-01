#include "GetCommand.h"
#include <cstring>

GetCommand::GetCommand(std::string key) : _key(std::move(key))
{
}

Response GetCommand::execute(IDatabase &db)
{
  Response resp;
  std::string value;

  if (db.get(_key, value))
  {
    resp.status = ResponseStatus::OK;
    resp.data.assign(value.begin(), value.end());
  }
  else
  {
    resp.status = ResponseStatus::NOT_FOUND;
  }

  return resp;
}
