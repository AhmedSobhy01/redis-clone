#include "command/KeysCommand.h"

Response KeysCommand::execute(IDatabase *db, const std::vector<std::string> &)
{
  std::vector<std::string> keys = db->keys();

  std::vector<Response> responseVector;
  for (const std::string &key : keys)
    responseVector.push_back(Response::str(key));

  return Response::array(responseVector);
}