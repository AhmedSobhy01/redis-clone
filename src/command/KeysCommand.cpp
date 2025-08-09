#include "command/KeysCommand.h"

KeysCommand::KeysCommand()
{
}

Response KeysCommand::execute(IDatabase &db)
{
  std::vector<std::string> keys = db.keys();

  std::vector<Response> responseVector;
  for (const auto &key : keys)
    responseVector.push_back(Response::str(key));

  return Response::array(responseVector);
}
