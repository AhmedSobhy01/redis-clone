#include "command/ZScoreCommand.h"
#include "containers/IDatabase.h"
#include "protocol/Response.h"
#include "containers/ZSetValue.h"

Response ZScoreCommand::execute(IDatabase *db, const std::vector<std::string> &args)
{
  if (args.size() != 3)
    return Response::error((uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT, "wrong number of arguments for 'zscore' command");

  std::shared_ptr<Value> value = db->get(args[1]);
  if (!value)
    return Response::nil();

  std::shared_ptr<ZSetValue> zsetValue = std::dynamic_pointer_cast<ZSetValue>(value);
  if (!zsetValue)
    return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a sorted set");

  double score;
  return zsetValue->score(args[2], score) ? Response::dbl(score) : Response::nil();
}
