#pragma once

#include "containers/TTLHeap.h"
#include "containers/IDatabase.h"

#include <unordered_map>
#include <cstdint>

const size_t kMaxExpiresWork = 2000;

class TTLManager
{
public:
  static TTLManager &instance();

  // prevent copy/move for singleton
  TTLManager(const TTLManager &) = delete;
  TTLManager &operator=(const TTLManager &) = delete;
  TTLManager(TTLManager &&) = delete;
  TTLManager &operator=(TTLManager &&) = delete;

  int getTTLByKey(const std::string &key);
  void addTTL(size_t *heapIndex, int ttl);
  void updateTTL(size_t heapIndex, int newTTL);
  void removeByKey(const std::string &key);
  void removeExpired(IDatabase *db);

private:
  TTLManager() = default;
  ~TTLManager() = default;

  TTLHeap _heap;
};
