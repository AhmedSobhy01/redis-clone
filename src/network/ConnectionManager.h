#pragma once

#include "Connection.h"

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

  std::vector<Connection *> getActiveConnections();
  Connection *getConnection(int fd);

private:
  std::vector<std::unique_ptr<Connection>> _connections;
  size_t _activeConnections;
};
