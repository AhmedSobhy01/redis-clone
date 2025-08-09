#include "command/ZAddCommand.h"
#include "containers/IDatabase.h"
#include "protocol/Response.h"
#include "utils/helpers.h"
#include "containers/ZSetValue.h"

#include <cmath>
#include <string>
#include <stdexcept>

Response ZAddCommand::execute(IDatabase *db, const std::vector<std::string> &args)
{
  if (args.size() != 4)
    return Response::error((uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT, "wrong number of arguments for 'zadd' command");

  double score;
  if (!parseDouble(args[2], score))
    return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a valid float");

  std::shared_ptr<Value> value = db->get(args[1]);
  if (!value)
    db->set(args[1], std::make_shared<ZSetValue>());

  value = db->get(args[1]);
  if (!value)
    return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a sorted set");

  std::shared_ptr<ZSetValue> zsetValue = std::dynamic_pointer_cast<ZSetValue>(value);
  if (!zsetValue)
    return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a sorted set");

  int64_t result = zsetValue->add(args[3], score);

  return Response::integer(result);
}
