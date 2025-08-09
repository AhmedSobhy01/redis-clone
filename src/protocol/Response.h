#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class ResponseTag : uint32_t
{
  TAG_NIL = 0, // nil
  TAG_ERR = 1, // error code + msg
  TAG_STR = 2, // string
  TAG_INT = 3, // int64
  TAG_DBL = 4, // double
  TAG_ARR = 5, // array
};

enum class ErrorCode : uint32_t
{
  ERR_UNKNOWN = 1,          // unknown command
  ERR_TOO_BIG = 2,          // response too big
  ERR_WRONG_ARGS_COUNT = 3, // wrong number of arguments
};

class Response
{
public:
  static Response nil();
  static Response str(const std::string &value);
  static Response integer(int64_t value);
  static Response error(uint32_t code, const std::string &message);
  static Response array(const std::vector<Response> &elements);

  void appendToBuffer(std::vector<uint8_t> &buffer) const;

private:
  Response(ResponseTag tag);
  Response(ResponseTag tag, const std::vector<uint8_t> &data);

  ResponseTag _tag;
  std::vector<uint8_t> _data;
};
