#include "server/Server.h"
#include "containers/ResizableHashTable.h"

#include <memory>
#include <iostream>
#include <signal.h>

#define DEFAULT_HOST "0.0.0.0";
#define DEFAULT_PORT 8080;

// handle sigint & sigterm
static Server *serverPtr = nullptr;
extern "C" void handleSignal(int)
{
  if (serverPtr)
    serverPtr->stop();
}

int main(int argc, char *argv[])
{
  std::string host = DEFAULT_HOST;
  uint32_t port = DEFAULT_PORT;

  if (argc > 2)
  {
    host = argv[1];
    port = std::atoi(argv[2]);

    if (port <= 0 || port > 65535)
    {
      std::cerr << "Invalid port: " << port << "\n";
      return 1;
    }
  }
  else
  {
    std::cout << "Using default host: " << host << " and port: " << port << "\n";
  }

  // setup signals
  struct sigaction sa{};
  sa.sa_handler = handleSignal;
  sigaction(SIGINT, &sa, nullptr);
  sigaction(SIGTERM, &sa, nullptr);

  try
  {
    std::unique_ptr<ResizableHashTable> db = std::make_unique<ResizableHashTable>(128);
    Server server(host, port, std::move(db));
    serverPtr = &server;

    std::cout << "Starting server on " << host << ":" << port << "\n";

    server.run();

    std::cout << "Shutting down server..." << std::endl;
  }
  catch (const std::exception &ex)
  {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }

  std::cout << "Server stopped\n";
  return 0;
}
