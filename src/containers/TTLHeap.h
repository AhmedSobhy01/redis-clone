#pragma once

#include <vector>
#include <chrono>
#include <string>

struct HeapEntry
{
  std::string key;
  std::chrono::steady_clock::time_point expiresAt;
  size_t *ttlHeapIndex;

  HeapEntry(std::chrono::steady_clock::time_point expiresAt, size_t *ttlHeapIndex);
};

class TTLHeap
{
public:
  ~TTLHeap();

  int getByKey(const std::string &key);
  void push(HeapEntry *entry);
  HeapEntry *pop();
  void removeByKey(const std::string &key);
  void updateTTL(size_t idx, std::chrono::steady_clock::time_point newTTL);

  bool empty() const;
  HeapEntry *top() const;

private:
  std::vector<HeapEntry *> _heap;

  void swapEntries(int i, int j);
  void heapifyUp(int i);
  void heapifyDown(int i);
};
