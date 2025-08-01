#include "ProtocolHelper.h"

#include <cstring>
#include <sstream>
#include <algorithm>

std::unique_ptr<Request> ProtocolHelper::parseRequest(ByteBuffer &buf)
{
  if (buf.size() < 4)
    return nullptr;

  // full msg length
  uint32_t msglen;
  std::memcpy(&msglen, buf.data(), 4);
  if (msglen > kMaxMsg)
    throw std::runtime_error("message too big");

  // msg not fully read yet
  if (buf.size() < msglen + 4)
    return nullptr;

  const uint8_t *p = buf.data() + 4;

  // no of command args
  uint32_t nstr;
  std::memcpy(&nstr, p, 4);
  p += 4;

  if (nstr > kMaxArgs)
    throw std::runtime_error("too many args");

  std::unique_ptr<Request> req = std::make_unique<Request>();
  for (uint32_t i = 0; i < nstr; i++)
  {
    // argument length
    uint32_t len;
    std::memcpy(&len, p, 4);
    p += 4;

    // argument data
    req->args.emplace_back(reinterpret_cast<const char *>(p), len);
    p += len;
  }
  buf.consume(4 + msglen);

  return req;
}

std::vector<uint8_t> ProtocolHelper::serialize(const Response &resp)
{
  // total length (4 bytes) | status (4 bytes) | payload
  uint32_t total = 4 + static_cast<uint32_t>(resp.data.size());

  if (total > kMaxMsg)
    throw std::runtime_error("response too big");

  std::vector<uint8_t> out;
  out.resize(4 + total);
  uint8_t *p = out.data();

  // total length
  std::memcpy(p, &total, 4);
  p += 4;

  // status code
  std::memcpy(p, &resp.status, 4);
  p += 4;

  // payload data
  std::memcpy(p, resp.data.data(), resp.data.size());

  return out;
}