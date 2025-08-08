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
      Response response;

      if (command)
        response = command->execute(_db);
      else
        response.status = ResponseStatus::ERROR;

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

    Response response;
    response.status = ResponseStatus::ERROR;

    std::vector<uint8_t> res = _protocolHelper.serialize(response);
    conn->outBuffer().append(res.data(), res.size());
    conn->setWantWrite(true);
  }
}
