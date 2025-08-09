#include "command/PTTLCommand.h"
#include "containers/HashTable.h"
#include "server/TTLManager.h"

Response PTTLCommand::execute(IDatabase *, const std::vector<std::string> &args)
{
  if (args.size() != 2)
    return Response::error((uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT, "wrong number of arguments for 'pttl' command");

  const std::string &key = args[1];

  int ttl = TTLManager::instance().getTTLByKey(key);

  return Response::integer(ttl);
}