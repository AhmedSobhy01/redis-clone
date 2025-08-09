#define container_of(ptr, type, member) \
  ((type *)((char *)(ptr) - offsetof(type, member)))

#include "server/TTLManager.h"
#include "containers/HashTable.h"

TTLManager &TTLManager::instance()
{
  static TTLManager _instance;
  return _instance;
}

int TTLManager::getTTLByKey(const std::string &key)
{
  return _heap.getByKey(key);
}

void TTLManager::addTTL(size_t *heapIndex, int ttl)
{
  HeapEntry *entry = new HeapEntry(std::chrono::steady_clock::now() + std::chrono::seconds(ttl), heapIndex);
  HashTable::Entry *htEntry = container_of(heapIndex, HashTable::Entry, ttlHeapIndex);
  entry->key = htEntry->key;
  _heap.push(entry);
}

void TTLManager::updateTTL(size_t heapIndex, int newTTL)
{
  _heap.updateTTL(heapIndex, std::chrono::steady_clock::now() + std::chrono::seconds(newTTL));
}

void TTLManager::removeByKey(const std::string &key)
{
  _heap.removeByKey(key);
}

void TTLManager::removeExpired(IDatabase *db)
{
  size_t workCount = 0;

  while (!_heap.empty() && workCount < kMaxExpiresWork)
  {
    HeapEntry *top = _heap.top();
    if (top->expiresAt > std::chrono::steady_clock::now())
      break;

    _heap.pop();

    HashTable::Entry *entry = container_of(top->ttlHeapIndex, HashTable::Entry, ttlHeapIndex);
    if (entry)
    {
      entry->ttlHeapIndex = (size_t)-1;
      db->del(entry->key);
    }

    delete top;

    workCount++;
  }
}
