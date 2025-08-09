#include "server/RequestProcessor.h"
#include "command/CommandFactory.h"

#include <iostream>

RequestProcessor::RequestProcessor(IDatabase &db) : _db(db)
{
}

void RequestProcessor::processRequests(Connection *conn)
{
  if (!conn || conn->isClosed())
    return;

  try
  {
    size_t processed = 0;
    while (processed < kMaxRequestsPerConnection)
    {
      std::unique_ptr<Request> request = _protocolHelper.parseRequest(conn->inBuffer());
      if (!request)
        break;

      // run command
      std::unique_ptr<Command> command = CommandFactory::create(*request);
      Response response = command
                              ? command->execute(_db)
                              : Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, "unknown command");

      // queue response
      std::vector<uint8_t> res = _protocolHelper.serialize(response);
      conn->outBuffer().append(res.data(), res.size());
      conn->setWantWrite(true);

      processed++;
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;

    Response response = Response::error((uint32_t)ErrorCode::ERR_UNKNOWN, e.what());

    std::vector<uint8_t> res = _protocolHelper.serialize(response);
    conn->outBuffer().append(res.data(), res.size());
    conn->setWantWrite(true);
  }
}
