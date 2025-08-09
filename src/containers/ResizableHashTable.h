#pragma once

#include "containers/HashTable.h"
#include "containers/IDatabase.h"

#include <memory>

const float kMaxLoadFactor = 8.0f;
const size_t kRehashingWorkSize = 128;

class ResizableHashTable : public IDatabase
{
public:
  ResizableHashTable(size_t initialSlots = 16);
  ~ResizableHashTable();

  bool get(const std::string &key, std::string &out) override;
  void set(const std::string &key, std::string value) override;
  bool del(const std::string &key) override;
  size_t size() const override;
  size_t capacity() const override;
  void clear() override;
  std::vector<std::string> keys() override;

private:
  std::unique_ptr<HashTable> _newer;
  std::unique_ptr<HashTable> _older;
  size_t _migratePos;

  void tryRehashing();
  void triggerRehashing();
  bool isRehashing() const;
  void migrateKeys();
};
