#include "EventLoop.h"

#include <poll.h>
#include <cerrno>
#include <stdexcept>
#include <vector>

EventLoop::EventLoop(const std::string &host, int port, ConnectionManager &connectionManager) : _connectionManager(connectionManager), _running(true)
{
  _listener = std::make_unique<Listener>(host, port, _connectionManager);
}

EventLoop::~EventLoop()
{
  _listener.reset();
}

void EventLoop::poll()
{
  if (!_running)
    return;

  std::vector<struct pollfd> tmp;
  tmp.clear();

  // listener
  struct pollfd listenerFD;
  listenerFD.fd = _listener->fd();
  listenerFD.events = POLLIN;
  listenerFD.revents = 0;
  tmp.push_back(listenerFD);

  // connections
  std::vector<Connection *> conns = _connectionManager.getActiveConnections();
  tmp.reserve(conns.size() + 1);

  for (Connection *conn : conns)
  {
    if (!conn || conn->isClosed())
      continue;

    struct pollfd pfd;
    pfd.fd = conn->fd();
    pfd.events = 0;
    pfd.revents = 0;

    pfd.events = POLLERR;
    if (conn->wantsRead())
      pfd.events |= POLLIN;
    if (conn->wantsWrite())
      pfd.events |= POLLOUT;

    tmp.push_back(pfd);
  }

  if (tmp.empty())
    return;

  // poll pfds with 100ms timeout
  int n = ::poll(tmp.data(), tmp.size(), 100);

  if (n < 0 && errno == EINTR)
    return;

  if (n < 0)
    throw std::runtime_error("poll failed");

  if (n == 0)
    return;

  // process events
  for (size_t i = 0; i < tmp.size() && n > 0; i++)
  {
    const pollfd &pfd = tmp[i];
    if (pfd.revents == 0)
      continue;

    if (i == 0) // listener
      handleListenerEvents(pfd);
    else // connection
      handleConnectionEvents(pfd);

    n--;
  }
}

void EventLoop::stop()
{
  _running = false;
}

bool EventLoop::isRunning() const
{
  return _running;
}

void EventLoop::handleListenerEvents(const struct pollfd &pfd)
{
  if (pfd.revents & POLLIN)
    _listener->onAccept();
}

void EventLoop::handleConnectionEvents(const struct pollfd &pfd)
{
  Connection *conn = _connectionManager.getConnection(pfd.fd);
  if (!conn)
    return;

  if (pfd.revents & POLLIN)
    conn->handleRead();

  if (pfd.revents & POLLOUT)
    conn->handleWrite();

  if (pfd.revents & (POLLERR | POLLHUP))
    conn->setWantClose(true);
}