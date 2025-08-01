#pragma once

#include <string>

class ConnectionManager;

class Listener
{
public:
  Listener(const std::string &host, int port, ConnectionManager &connectionManager);
  ~Listener();

  int fd() const;
  void onAccept();

private:
  int _listen_fd;
  ConnectionManager &_connectionManager;
};
