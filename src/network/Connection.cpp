#include "network/Connection.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <algorithm>

Connection::Connection(int fd) : _fd(fd), _want_read(false), _want_write(false), _want_close(false), _lastActivity(std::chrono::steady_clock::now())
{
  ::fcntl(fd, F_SETFL, ::fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

Connection::~Connection()
{
  if (_fd >= 0)
  {
    ::close(_fd);
    _fd = -1;
  }
}

void Connection::handleRead()
{
  uint8_t buf[kReadBufferSize];
  ssize_t n = ::read(_fd, buf, sizeof(buf));

  if (n < 0 && errno == EAGAIN)
    return;

  if (n <= 0)
  {
    _want_close = true;
    return;
  }

  updateActivity();
  _in.append(buf, n);
}

void Connection::handleWrite()
{
  if (_out.size() == 0)
    return;

  ssize_t n = ::write(_fd, _out.data(), _out.size());

  if (n < 0 && errno == EAGAIN)
    return;

  if (n <= 0)
  {
    _want_close = true;
    return;
  }

  updateActivity();
  _out.consume(n);

  // if done, go back to read
  if (_out.size() == 0)
    setWantRead(true);
}

bool Connection::wantsRead() const
{
  return _want_read && !_want_close;
}

bool Connection::wantsWrite() const
{
  return _want_write && !_want_close;
}

bool Connection::isClosed() const
{
  return _want_close;
}

bool Connection::isTimedOut(std::chrono::steady_clock::time_point now, std::chrono::seconds timeout) const
{
  return (now - _lastActivity) > timeout;
}

void Connection::setWantRead(bool want)
{
  _want_write = false;
  _want_read = want;
}

void Connection::setWantWrite(bool want)
{
  _want_read = false;
  _want_write = want;
}

void Connection::setWantClose(bool want)
{
  _want_close = want;
}

ByteBuffer &Connection::inBuffer()
{
  return _in;
}

ByteBuffer &Connection::outBuffer()
{
  return _out;
}

int Connection::fd() const
{
  return _fd;
}

void Connection::updateActivity()
{
  _lastActivity = std::chrono::steady_clock::now();
}