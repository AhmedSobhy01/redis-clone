#pragma once

#include "network/ConnectionManager.h"
#include "network/Listener.h"

#include <memory>
#include <string>
#include <vector>
#include <sys/epoll.h>

constexpr int kMaxEvents = 128;

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
  int _epoll_fd;

  void handleListenerEvents(uint32_t events);
  void handleConnectionEvents(int fd, uint32_t events);
};
