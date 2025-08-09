#pragma once

#include "containers/AVLNode.h"

class AVLTree
{
public:
  AVLTree();
  ~AVLTree();

  AVLNode *insert(std::shared_ptr<Value> value);
  AVLNode *find(const std::shared_ptr<Value> &value) const;
  bool remove(const std::shared_ptr<Value> &value);
  AVLNode *findByIndex(uint32_t index) const;

  AVLNode *root() const;
  uint32_t size() const;

  AVLNode *first() const;
  AVLNode *last() const;
  AVLNode *next(AVLNode *node) const;
  AVLNode *prev(AVLNode *node) const;

private:
  AVLNode *_root;
};