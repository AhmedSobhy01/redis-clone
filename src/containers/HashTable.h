#pragma once

#include "IDatabase.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class HashTable : public IDatabase
{
public:
  HashTable(size_t initial_slots = 16);
  ~HashTable();

  bool get(const std::string &key, std::string &out) override;
  void set(const std::string &key, std::string value) override;
  bool del(const std::string &key) override;
  size_t size() const override;
  void clear() override;

private:
  struct Entry
  {
    uint64_t hashCode;
    std::string key;
    std::string val;
    Entry *next;
  };

  std::vector<Entry *> _slots;
  size_t _mask;
  size_t _size;

  uint64_t hashKey(const std::string &key) const;

  friend class ResizableHashTable;
};
