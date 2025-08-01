#pragma once

#include "../containers/IDatabase.h"
#include "../network/Connection.h"
#include "../protocol/ProtocolHelper.h"

const size_t kMaxRequestsPerConnection = 100;

class RequestProcessor
{
public:
  RequestProcessor(IDatabase &db);
  void processRequests(Connection *conn);

private:
  IDatabase &_db;
  ProtocolHelper _protocolHelper;
};
