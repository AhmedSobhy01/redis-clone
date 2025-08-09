#include "command/PExpireCommand.h"
#include "containers/HashTable.h"
#include "containers/ResizableHashTable.h"
#include "server/TTLManager.h"

Response PExpireCommand::execute(IDatabase *db, const std::vector<std::string> &args)
{
  if (args.size() != 3)
    return Response::error((uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT, "wrong number of arguments for 'pexpire' command");

  const std::string &key = args[1];
  int64_t ttlMs = std::stoll(args[2]);
  if (ttlMs <= 0)
    return Response::integer(0);

  auto *rdb = dynamic_cast<ResizableHashTable *>(db);
  if (!rdb)
    return Response::integer(0);

  HashTable::Entry *entry = rdb->findEntry(key);
  if (!entry)
    return Response::integer(0);

  if (entry->ttlHeapIndex == (size_t)-1)
    TTLManager::instance().addTTL(&entry->ttlHeapIndex, static_cast<int>(ttlMs / 1000));
  else
    TTLManager::instance().updateTTL(entry->ttlHeapIndex, static_cast<int>(ttlMs / 1000));

  return Response::integer(1);
}