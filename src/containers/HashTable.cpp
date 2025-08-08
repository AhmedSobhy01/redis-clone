#include "HashTable.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <string>

HashTable::HashTable(size_t slotsCount) : _slots(), _mask(slotsCount - 1), _size(0)
{
  assert(slotsCount > 0 && (slotsCount & _mask) == 0 && "slotsCount must be a power of two");

  // usage of calloc instead of new or malloc is to avoid the O(N) initialization cost
  // by progressively initializing it instead
  _slots = (Entry **)calloc(slotsCount, sizeof(Entry *));
}

HashTable::~HashTable()
{
  clear();
  free(_slots);
}

bool HashTable::get(const std::string &key, std::string &out)
{
  uint64_t h = hashKey(key);
  size_t idx = h & _mask;

  Entry *e = _slots[idx];
  while (e)
  {
    if (e->hashCode == h && e->key == key)
    {
      out = e->val;
      return true;
    }

    e = e->next;
  }

  return false;
}

void HashTable::set(const std::string &key, std::string value)
{
  uint64_t h = hashKey(key);
  size_t idx = h & _mask;

  Entry *e = _slots[idx];
  while (e)
  {
    if (e->hashCode == h && e->key == key)
    {
      e->val = std::move(value);
      return;
    }

    e = e->next;
  }

  _slots[idx] = new Entry{h, key, std::move(value), _slots[idx]};
  _size++;
}

bool HashTable::del(const std::string &key)
{
  uint64_t h = hashKey(key);
  size_t idx = h & _mask;

  Entry *prev = nullptr;
  Entry *e = _slots[idx];
  while (e)
  {
    if (e->hashCode == h && e->key == key)
    {
      if (prev)
        prev->next = e->next;
      else
        _slots[idx] = e->next;
      delete e;

      _size--;

      return true;
    }

    prev = e;
    e = e->next;
  }

  return false;
}

size_t HashTable::size() const
{
  return _size;
}

size_t HashTable::capacity() const
{
  return _mask + 1; // since _mask is slotsCount - 1
}

void HashTable::clear()
{
  for (size_t i = 0; i <= _mask; ++i)
  {
    Entry *e = _slots[i];
    while (e)
    {
      Entry *next = e->next;
      delete e;
      e = next;
    }
    _slots[i] = nullptr;
  }

  _size = 0;
}

uint64_t HashTable::hashKey(const std::string &key) const
{
  // FNV-1a hash function
  // http://www.isthe.com/chongo/tech/comp/fnv/
  const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
  const uint64_t FNV_PRIME = 1099511628211ULL;

  uint64_t hash = FNV_OFFSET_BASIS;
  for (char c : key)
  {
    hash ^= static_cast<uint64_t>(c);
    hash *= FNV_PRIME;
  }

  return hash;
}
