#pragma once

#include "network/EventLoop.h"
#include "network/ConnectionManager.h"
#include "containers/IDatabase.h"
#include "server/RequestProcessor.h"

#include <memory>
#include <chrono>

const std::chrono::seconds kCleanupInterval{30};
const std::chrono::seconds kConnectionTimeout{300};

class Server
{
public:
  Server(const std::string &host, int port, std::unique_ptr<IDatabase> db);
  ~Server();

  void stop();
  void run();

  bool isRunning() const;

private:
  std::unique_ptr<EventLoop> _eventLoop;
  std::unique_ptr<ConnectionManager> _connectionManager;
  std::unique_ptr<IDatabase> _database;
  std::unique_ptr<RequestProcessor> _requestProcessor;

  bool _running;
  std::chrono::steady_clock::time_point _lastCleanup;
};
