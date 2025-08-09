#include "protocol/ProtocolHelper.h"

#include <cstring>
#include <stdexcept>

std::unique_ptr<Request> ProtocolHelper::parseRequest(ByteBuffer &buf)
{
  // nstr (4 bytes) | len (4 bytes) | str1 | len (4 bytes) | str2 | ... | len (4 bytes) | strn

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
  std::vector<uint8_t> out;

  // space for message length
  uint32_t totalLen = 0;
  out.resize(4);
  std::memcpy(out.data(), &totalLen, 4);

  resp.appendToBuffer(out);

  // update message length
  size_t msgSize = out.size() - 4;

  if (msgSize > kMaxMsg)
  {
    out.resize(4);
    Response::error((uint32_t)ErrorCode::ERR_TOO_BIG, "response is too big.").appendToBuffer(out);
    msgSize = out.size() - 4;
  }

  // update message length
  uint32_t len = (uint32_t)msgSize;
  std::memcpy(out.data(), &len, 4);

  return out;
}