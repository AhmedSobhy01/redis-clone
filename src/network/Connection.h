#pragma once

#include "protocol/ByteBuffer.h"

#include <functional>
#include <chrono>

const size_t kReadBufferSize = 64 * 1024;

class Connection
{
public:
  Connection(int fd);
  ~Connection();

  void handleRead();
  void handleWrite();

  bool wantsRead() const;
  bool wantsWrite() const;
  bool isClosed() const;
  bool isTimedOut(std::chrono::steady_clock::time_point now, std::chrono::seconds timeout) const;

  void setWantRead(bool want);
  void setWantWrite(bool want);
  void setWantClose(bool want);

  ByteBuffer &inBuffer();
  ByteBuffer &outBuffer();
  int fd() const;

private:
  int _fd;
  ByteBuffer _in;
  ByteBuffer _out;
  bool _want_read;
  bool _want_write;
  bool _want_close;
  std::chrono::steady_clock::time_point _lastActivity;

  void updateActivity();
};
