#include "protocol/Response.h"

#include <cstring>

Response::Response(ResponseTag tag) : _tag(tag)
{
}

Response::Response(ResponseTag tag, const std::vector<uint8_t> &data) : _tag(tag), _data(data)
{
}

Response Response::nil()
{
  return Response(ResponseTag::TAG_NIL);
}

Response Response::str(const std::string &value)
{
  std::vector<uint8_t> data;
  uint32_t len = static_cast<uint32_t>(value.size());

  // string length
  data.resize(4);
  std::memcpy(data.data(), &len, 4);

  // string data
  data.insert(data.end(), value.begin(), value.end());

  return Response(ResponseTag::TAG_STR, data);
}

Response Response::integer(int64_t value)
{
  std::vector<uint8_t> data(8);
  std::memcpy(data.data(), &value, 8);

  return Response(ResponseTag::TAG_INT, data);
}

Response Response::error(uint32_t code, const std::string &message)
{
  std::vector<uint8_t> data;

  // error code
  data.resize(4);
  std::memcpy(data.data(), &code, 4);

  // error message length
  uint32_t len = static_cast<uint32_t>(message.size());
  data.resize(8);
  std::memcpy(data.data() + 4, &len, 4);

  // message string
  data.insert(data.end(), message.begin(), message.end());

  return Response(ResponseTag::TAG_ERR, data);
}

Response Response::array(uint32_t count)
{
  std::vector<uint8_t> data(4);
  std::memcpy(data.data(), &count, 4);

  return Response(ResponseTag::TAG_ARR, data);
}

Response Response::array(const std::vector<std::string> &elements)
{
  std::vector<uint8_t> data(4);
  uint32_t count = static_cast<uint32_t>(elements.size());
  std::memcpy(data.data(), &count, 4);

  // serialize array elements
  for (const std::string &element : elements)
  {
    uint32_t strLen = static_cast<uint32_t>(element.size());
    data.resize(data.size() + 4 + strLen);
    std::memcpy(data.data() + data.size() - 4 - strLen, &strLen, 4);
    std::memcpy(data.data() + data.size() - strLen, element.data(), strLen);
  }

  return Response(ResponseTag::TAG_ARR, data);
}

void Response::appendToBuffer(std::vector<uint8_t> &buffer) const
{
  size_t totalSize = 1 + _data.size(); // 1 byte for tag + data size

  size_t oldSize = buffer.size();
  buffer.resize(oldSize + totalSize);

  uint8_t *dest = buffer.data() + oldSize;

  // tag
  *dest = (uint8_t)_tag;
  dest++;

  // data
  if (!_data.empty())
    std::memcpy(dest, _data.data(), _data.size());
}