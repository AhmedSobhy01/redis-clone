#include "server/Server.h"

#include <iostream>

Server::Server(const std::string &host, int port, std::unique_ptr<IDatabase> db) : _database(std::move(db)), _running(true), _lastCleanup(std::chrono::steady_clock::now())
{
  _connectionManager = std::make_unique<ConnectionManager>();
  _eventLoop = std::make_unique<EventLoop>(host, port, *_connectionManager);
  _requestProcessor = std::make_unique<RequestProcessor>(*_database);
}

Server::~Server()
{
  stop();
}

void Server::stop()
{
  _running = false;
  if (_eventLoop)
    _eventLoop->stop();

  if (_connectionManager)
    _connectionManager->cleanupConnections(std::chrono::seconds(0)); // timeouts all conns and closes them
}

void Server::run()
{
  while (_running)
  {
    // read, write events on connections and listener
    _eventLoop->poll();

    // processes requests
    const std::vector<Connection *> &activeConnections = _connectionManager->getActiveConnections();

    for (Connection *conn : activeConnections)
    {
      if (conn && !conn->isClosed())
        _requestProcessor->processRequests(conn);

      // check for timeouts or closed connections
      _connectionManager->cleanupConnection(conn, kConnectionTimeout);
    }
  }
}

bool Server::isRunning() const
{
  return _running;
}
