#pragma once

#include "containers/IDatabase.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class Value;

class HashTable : public IDatabase
{
public:
  HashTable(size_t initial_slots = 16);
  ~HashTable();

  std::shared_ptr<Value> get(const std::string &key) override;
  void set(const std::string &key, std::shared_ptr<Value> value) override;
  bool del(const std::string &key) override;
  size_t size() const override;
  size_t capacity() const override;
  void clear() override;
  std::vector<std::string> keys() override;

private:
  struct Entry
  {
    uint64_t hashCode;
    std::string key;
    std::shared_ptr<Value> val;
    Entry *next;
  };

  Entry **_slots;
  size_t _mask;
  size_t _size;

  uint64_t hashKey(const std::string &key) const;

  friend class ResizableHashTable;
};
