#include "command/GetCommand.h"
#include "containers/Value.h"
#include "containers/StringValue.h"

Response GetCommand::execute(IDatabase *db, const std::vector<std::string> &args)
{
  if (args.size() != 2)
    return Response::error((uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT, "wrong number of arguments for 'get' command");

  const std::string &key = args[1];
  std::shared_ptr<Value> value = db->get(key);

  if (value)
  {
    StringValue *strValue = dynamic_cast<StringValue *>(value.get());

    if (strValue)
      return Response::str(strValue->getValue());
    else
      return Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "value is not a string");
  }

  return Response::nil();
}