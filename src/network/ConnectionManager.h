#pragma once

#include "network/Connection.h"

#include <vector>
#include <memory>
#include <chrono>

class ConnectionManager
{
public:
  ConnectionManager();
  ~ConnectionManager();

  void addConnection(int fd);
  void removeConnection(int fd);
  void cleanupConnections(std::chrono::seconds timeout);

  const std::vector<Connection *> &getActiveConnections();
  Connection *getConnection(int fd);

private:
  std::vector<std::unique_ptr<Connection>> _connections;
  size_t _activeConnections;

  std::vector<Connection *> _activeConnectionsCache;
  bool _cacheValid;

  size_t getSystemMaxFD() const;
};
