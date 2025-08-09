#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

class AVLNode
{
public:
  AVLNode();
  virtual ~AVLNode() = default;

  AVLNode *fix();
  AVLNode *remove();

  uint32_t height() const { return _height; }
  uint32_t count() const { return _count; }
  AVLNode *parent() const { return _parent; }
  AVLNode *left() const { return _left; }
  AVLNode *right() const { return _right; }

protected:
  AVLNode *_parent = nullptr;
  AVLNode *_left = nullptr;
  AVLNode *_right = nullptr;
  uint32_t _height = 1;
  uint32_t _count = 1;
  std::string val;

private:
  void update();
  AVLNode *rotateLeft();
  AVLNode *rotateRight();
  AVLNode *fixLeft();
  AVLNode *fixRight();
  AVLNode *removeWithOneOrNoneChild();

  static uint32_t getHeight(AVLNode *node) { return node ? node->_height : 0; }
  static uint32_t getCount(AVLNode *node) { return node ? node->_count : 0; }
  static uint32_t max(uint32_t lhs, uint32_t rhs) { return lhs < rhs ? rhs : lhs; }

  friend class AVLTree;
};

class AVLTree
{
public:
  AVLTree() : _root(nullptr) {}
  ~AVLTree() = default;

  AVLNode *root() const { return _root; }
  void setRoot(AVLNode *root) { _root = root; }

private:
  AVLNode *_root;
};
