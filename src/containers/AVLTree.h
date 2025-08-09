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
  AVLNode *lowerBound(const std::shared_ptr<Value> &value) const;
  AVLNode *next(AVLNode *node) const;
  AVLNode *prev(AVLNode *node) const;

private:
  AVLNode *_root;
};