#include "DelCommand.h"

DelCommand::DelCommand(std::string key) : _key(std::move(key))
{
}

Response DelCommand::execute(IDatabase &db)
{
  Response resp;

  if (db.del(_key))
    resp.status = ResponseStatus::OK;
  else
    resp.status = ResponseStatus::NOT_FOUND;

  return resp;
}
