#pragma once

#include "network/ConnectionManager.h"
#include "network/Listener.h"

#include <memory>
#include <string>
#include <vector>

struct pollfd;

class EventLoop
{
public:
  EventLoop(const std::string &host, int port, ConnectionManager &connectionManager);
  ~EventLoop();

  void poll();
  void stop();
  bool isRunning() const;

private:
  std::unique_ptr<Listener> _listener;
  ConnectionManager &_connectionManager;
  bool _running;

  void handleListenerEvents(const struct pollfd &pfd);
  void handleConnectionEvents(const struct pollfd &pfd);
};
