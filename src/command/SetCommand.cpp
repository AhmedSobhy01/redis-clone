#include "command/SetCommand.h"
#include "containers/Value.h"
#include "containers/StringValue.h"

Response SetCommand::execute(IDatabase *db, const std::vector<std::string> &args)
{
  if (args.size() != 3)
    return Response::error((uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT, "wrong number of arguments for 'set' command");

  const std::string &key = args[1];
  const std::string &value = args[2];

  db->set(key, std::make_shared<StringValue>(value));

  return Response::nil();
}