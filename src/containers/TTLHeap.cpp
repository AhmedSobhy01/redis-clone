#include "containers/TTLHeap.h"

#include <utility>
#include "TTLHeap.h"

HeapEntry::HeapEntry(std::chrono::steady_clock::time_point expiresAt, size_t *ttlHeapIndex) : expiresAt(expiresAt), ttlHeapIndex(ttlHeapIndex) {}

TTLHeap::~TTLHeap()
{
  for (HeapEntry *entry : _heap)
    delete entry;
}

int TTLHeap::getByKey(const std::string &key)
{
  for (const HeapEntry *entry : _heap)
    if (entry->key == key)
      return std::chrono::duration_cast<std::chrono::milliseconds>(entry->expiresAt - std::chrono::steady_clock::now()).count();

  return -1;
}

void TTLHeap::push(HeapEntry *entry)
{
  _heap.push_back(entry);
  int idx = _heap.size() - 1;
  *entry->ttlHeapIndex = idx;
  heapifyUp(idx);
}

HeapEntry *TTLHeap::pop()
{
  if (_heap.empty())
    return nullptr;

  HeapEntry *top = _heap[0];
  HeapEntry *last = _heap.back();
  _heap.pop_back();

  if (!_heap.empty())
  {
    _heap[0] = last;
    *last->ttlHeapIndex = 0;
    heapifyDown(0);
  }

  return top;
}

void TTLHeap::removeByKey(const std::string &key)
{
  for (size_t i = 0; i < _heap.size(); ++i)
  {
    if (_heap[i]->key == key)
    {
      delete _heap[i];

      swapEntries(i, _heap.size() - 1);
      _heap.pop_back();

      if (i < _heap.size())
      {
        heapifyDown(i);
        heapifyUp(i);
      }

      return;
    }
  }
}

void TTLHeap::updateTTL(size_t idx, std::chrono::steady_clock::time_point newTTL)
{
  std::chrono::steady_clock::time_point oldTTL = _heap[idx]->expiresAt;
  _heap[idx]->expiresAt = newTTL;

  if (newTTL < oldTTL)
    heapifyUp(idx);
  else if (newTTL > oldTTL)
    heapifyDown(idx);
}

bool TTLHeap::empty() const
{
  return _heap.empty();
}

HeapEntry *TTLHeap::top() const
{
  if (_heap.empty())
    return nullptr;

  return _heap[0];
}

void TTLHeap::swapEntries(int i, int j)
{
  std::swap(_heap[i], _heap[j]);
  *_heap[i]->ttlHeapIndex = i;
  *_heap[j]->ttlHeapIndex = j;
}

void TTLHeap::heapifyUp(int i)
{
  while (i > 0)
  {
    int parent = (i - 1) / 2;
    if (_heap[i]->expiresAt < _heap[parent]->expiresAt)
    {
      swapEntries(i, parent);
      i = parent;
    }
    else
      break;
  }
}

void TTLHeap::heapifyDown(int i)
{
  int n = _heap.size();
  while (true)
  {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int smallest = i;

    if (left < n && _heap[left]->expiresAt < _heap[smallest]->expiresAt)
      smallest = left;
    if (right < n && _heap[right]->expiresAt < _heap[smallest]->expiresAt)
      smallest = right;

    if (smallest != i)
    {
      swapEntries(i, smallest);
      i = smallest;
    }
    else
      break;
  }
}
