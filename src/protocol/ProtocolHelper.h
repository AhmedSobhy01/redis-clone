#pragma once

#include "ByteBuffer.h"
#include "Request.h"
#include "Response.h"

#include <memory>

const uint32_t kMaxArgs = 100;
const uint32_t kMaxMsg = 10 * 1024 * 1024;

class ProtocolHelper
{
public:
  std::unique_ptr<Request> parseRequest(ByteBuffer &buf);
  std::vector<uint8_t> serialize(const Response &resp);
};
