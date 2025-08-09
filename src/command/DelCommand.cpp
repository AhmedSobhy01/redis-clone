#include "command/DelCommand.h"

Response DelCommand::execute(IDatabase *db, const std::vector<std::string> &args)
{
  if (args.size() != 2)
    return Response::error((uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT, "wrong number of arguments for 'del' command");

  const std::string &key = args[1];
  bool deleted = db->del(key);

  return Response::integer(deleted ? 1 : 0);
}