#pragma once

#include "containers/Value.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>
#include <algorithm>

class AVLTree;

class AVLNode
{
public:
  AVLNode(std::shared_ptr<Value> value);
  ~AVLNode();

  uint32_t height() const { return _height; }
  uint32_t count() const { return _count; }
  AVLNode *parent() const { return _parent; }
  AVLNode *left() const { return _left; }
  AVLNode *right() const { return _right; }
  std::shared_ptr<Value> value() const { return val; }

private:
  AVLNode *fix();
  AVLNode *remove();

  AVLNode *_parent = nullptr;
  AVLNode *_left = nullptr;
  AVLNode *_right = nullptr;
  uint32_t _height = 1;
  uint32_t _count = 1;
  std::shared_ptr<Value> val;

  void update();
  AVLNode *rotateLeft();
  AVLNode *rotateRight();
  AVLNode *fixLeft();
  AVLNode *fixRight();
  AVLNode *removeWithOneOrNoneChild();

  static uint32_t getHeight(AVLNode *node) { return node ? node->_height : 0; }
  static uint32_t getCount(AVLNode *node) { return node ? node->_count : 0; }

  friend class AVLTree;
};
