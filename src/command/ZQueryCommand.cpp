#include "command/ZQueryCommand.h"
#include "containers/IDatabase.h"
#include "protocol/Response.h"
#include "utils/helpers.h"
#include "containers/ZSetValue.h"

#include <cmath>

Response ZQueryCommand::execute(IDatabase *db, const std::vector<std::string> &args)
{
  if (args.size() != 6)
    return Response::error((uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT, "wrong number of arguments for 'zquery' command");

  double score;
  if (!parseDouble(args[2], score))
    return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a valid float");

  int offset;
  if (!parseInteger(args[4], offset))
    return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a valid integer");

  int limit;
  if (!parseInteger(args[5], limit))
    return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a valid integer");

  if (limit <= 0)
    return Response::array({});

  std::shared_ptr<Value> value = db->get(args[1]);
  if (!value)
    return Response::array({});

  std::shared_ptr<ZSetValue> zsetValue = std::dynamic_pointer_cast<ZSetValue>(value);
  if (!zsetValue)
    return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a sorted set");

  std::vector<std::pair<std::string, double>> results;
  zsetValue->query(score, args[3], offset, limit, results);

  std::vector<Response> arr;
  for (const std::pair<std::string, double> &result : results)
  {
    arr.push_back(Response::str(result.first));
    arr.push_back(Response::dbl(result.second));
  }

  return Response::array(arr);
}
