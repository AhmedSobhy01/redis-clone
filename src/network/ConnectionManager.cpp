#include "ConnectionManager.h"

#include <unistd.h>
#include <algorithm>
#include <sys/resource.h>

ConnectionManager::ConnectionManager() : _activeConnections(0), _cacheValid(false)
{
  _connections.reserve(getSystemMaxFD());
}

ConnectionManager::~ConnectionManager()
{
  for (std::unique_ptr<Connection> &conn : _connections)
    if (conn)
      ::close(conn->fd());
}

void ConnectionManager::addConnection(int fd)
{
  if (int(_connections.size()) < fd + 1)
    _connections.resize(std::max(fd + 1, int(_connections.size()) << 1));

  std::unique_ptr<Connection> conn = std::make_unique<Connection>(fd);
  _connections[fd] = std::move(conn);
  _connections[fd]->setWantRead(true);
  _activeConnections++;
  _cacheValid = false;
}

void ConnectionManager::removeConnection(int fd)
{
  if (fd >= 0 && fd < int(_connections.size()) && _connections[fd])
  {
    ::close(_connections[fd]->fd());
    _connections[fd].reset();
    _activeConnections--;
    _cacheValid = false;
  }
}

void ConnectionManager::cleanupConnections(std::chrono::seconds timeout)
{
  auto now = std::chrono::steady_clock::now();
  bool change = false;

  for (std::unique_ptr<Connection> &conn : _connections)
  {
    if (conn && (conn->isClosed() || conn->isTimedOut(now, timeout)))
    {
      int fd = conn->fd();
      ::close(fd);
      conn.reset();
      _activeConnections--;
      change = true;
    }
  }

  if (change)
    _cacheValid = false;
}

const std::vector<Connection *> &ConnectionManager::getActiveConnections()
{
  if (!_cacheValid)
  {
    _activeConnectionsCache.clear();
    _activeConnectionsCache.reserve(_activeConnections);

    for (std::unique_ptr<Connection> &conn : _connections)
      if (conn && !conn->isClosed())
        _activeConnectionsCache.push_back(conn.get());

    _cacheValid = true;
  }

  return _activeConnectionsCache;
}

Connection *ConnectionManager::getConnection(int fd)
{
  if (fd >= 0 && fd < int(_connections.size()))
    return _connections[fd].get();

  return nullptr;
}

size_t ConnectionManager::getSystemMaxFD() const
{
  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) == 0)
    return std::min(rl.rlim_cur, (size_t)65536); // limit it to 65536

  return 1024;
}
