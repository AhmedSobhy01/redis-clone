#include "containers/ResizableHashTable.h"

#include <vector>
#include <cassert>

ResizableHashTable::ResizableHashTable(size_t initialSlots) : _older(nullptr), _migratePos(0)
{
  _newer = std::make_unique<HashTable>(initialSlots);
}

ResizableHashTable::~ResizableHashTable()
{
}

std::shared_ptr<Value> ResizableHashTable::get(const std::string &key)
{
  // check new table first
  if (std::shared_ptr<Value> result = _newer->get(key))
  {
    if (isRehashing())
      tryRehashing();
    return result;
  }

  // check old table if still rehashing
  if (isRehashing())
  {
    std::shared_ptr<Value> result = _older->get(key);
    tryRehashing();
    return result;
  }

  return nullptr;
}

void ResizableHashTable::set(const std::string &key, std::shared_ptr<Value> value)
{
  _newer->set(key, value);

  // delete from old table
  if (isRehashing())
  {
    _older->del(key);
    tryRehashing();
  }
  else
  {
    // check if rehashing is needed
    if (_newer->size() > kMaxLoadFactor * _newer->capacity())
      triggerRehashing();
  }
}

bool ResizableHashTable::del(const std::string &key)
{
  // try new table first
  if (_newer->del(key))
  {
    if (isRehashing())
      tryRehashing();

    return true;
  }

  // try old table
  if (isRehashing())
  {
    bool result = _older->del(key);
    tryRehashing();
    return result;
  }

  return false;
}

size_t ResizableHashTable::size() const
{
  return _newer->size() + (isRehashing() ? _older->size() : 0);
}

size_t ResizableHashTable::capacity() const
{
  return _newer->capacity();
}

void ResizableHashTable::clear()
{
  _newer->clear();
  _older.reset();

  _migratePos = 0;
}

std::vector<std::string> ResizableHashTable::keys()
{
  std::vector<std::string> keys;

  // new table
  std::vector<std::string> newKeys = _newer->keys();
  keys.insert(keys.end(), newKeys.begin(), newKeys.end());

  // old table
  if (isRehashing())
  {
    std::vector<std::string> oldKeys = _older->keys();
    keys.insert(keys.end(), oldKeys.begin(), oldKeys.end());
  }

  return keys;
}

void ResizableHashTable::tryRehashing()
{
  assert(isRehashing() && "Not rehashing, cannot migrate keys");

  migrateKeys();

  // we are done
  if (_older->size() == 0)
  {
    _older.reset();
    _migratePos = 0;
  }
}

void ResizableHashTable::triggerRehashing()
{
  assert(!isRehashing());

  _older = std::move(_newer);
  _migratePos = 0;

  _newer = std::make_unique<HashTable>(_older->capacity() << 1); // double the size
  tryRehashing();
}

bool ResizableHashTable::isRehashing() const
{
  return _older != nullptr && _migratePos < _older->capacity();
}

void ResizableHashTable::migrateKeys()
{
  assert(isRehashing() && "Not rehashing, cannot migrate keys");

  size_t migrated = 0;
  while (migrated < kRehashingWorkSize && _older->size() > 0)
  {
    // find a filled slot
    while (_migratePos < _older->capacity() && _older->_slots[_migratePos] == nullptr)
      _migratePos++;

    if (_migratePos >= _older->capacity())
      break;

    HashTable::Entry *entry = _older->_slots[_migratePos];
    if (!entry)
      break;

    _older->_slots[_migratePos] = entry->next;
    entry->next = nullptr;
    _older->_size--;

    // insert into the new table (no need to change ref count - just moving ownership)
    size_t idx = entry->hashCode & _newer->_mask;
    entry->next = _newer->_slots[idx];
    _newer->_slots[idx] = entry;
    _newer->_size++;

    migrated++;
  }
}
