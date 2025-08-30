#include "network/EventLoop.h"

#include <sys/epoll.h>
#include <cerrno>
#include <stdexcept>
#include <vector>
#include <unistd.h>

EventLoop::EventLoop(const std::string &host, int port, ConnectionManager &connectionManager) : _connectionManager(connectionManager), _running(true)
{
  _listener = std::make_unique<Listener>(host, port, _connectionManager);

  _epoll_fd = ::epoll_create1(0);
  if (_epoll_fd < 0)
    throw std::runtime_error("epoll_create1 failed");

  // add listener to epoll
  struct epoll_event ev{};
  ev.events = EPOLLIN;
  ev.data.fd = _listener->fd();
  if (::epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _listener->fd(), &ev) < 0)
    throw std::runtime_error("epoll_ctl add listener failed");
}

EventLoop::~EventLoop()
{
  if (_epoll_fd >= 0)
    ::close(_epoll_fd);

  _listener.reset();
}

void EventLoop::poll()
{
  if (!_running)
    return;

  // connections
  const std::vector<Connection *> &conns = _connectionManager.getActiveConnections();
  for (Connection *conn : conns)
  {
    if (!conn || conn->isClosed())
      continue;

    struct epoll_event ev{};
    ev.data.fd = conn->fd();
    ev.events = EPOLLERR;

    if (conn->wantsRead())
      ev.events |= EPOLLIN;
    if (conn->wantsWrite())
      ev.events |= EPOLLOUT;

    // update in epoll
    if (::epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, conn->fd(), &ev) < 0)
      if (errno == ENOENT && ::epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, conn->fd(), &ev) < 0)
        continue;
  }

  struct epoll_event events[kMaxEvents];

  int n = ::epoll_wait(_epoll_fd, events, kMaxEvents, 100);

  if (n < 0 && errno == EINTR)
    return;

  if (n < 0)
    throw std::runtime_error("epoll_wait failed");

  if (n == 0)
    return;

  // process events
  for (int i = 0; i < n; i++)
  {
    int fd = events[i].data.fd;
    uint32_t ev = events[i].events;

    if (fd == _listener->fd())
      handleListenerEvents(ev);
    else
      handleConnectionEvents(fd, ev);
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

void EventLoop::handleListenerEvents(uint32_t events)
{
  if (events & EPOLLIN)
    _listener->onAccept();
}

void EventLoop::handleConnectionEvents(int fd, uint32_t events)
{
  Connection *conn = _connectionManager.getConnection(fd);
  if (!conn)
    return;

  if (events & EPOLLIN)
    conn->handleRead();

  if (events & EPOLLOUT)
    conn->handleWrite();

  if (events & (EPOLLERR | EPOLLHUP))
  {
    conn->setWantClose(true);
    ::epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
  }
}