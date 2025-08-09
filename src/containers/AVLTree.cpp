#include "containers/AVLTree.h"

AVLTree::AVLTree() : _root(nullptr) {}

AVLTree::~AVLTree()
{
  if (_root)
    delete _root;
}

AVLNode *AVLTree::insert(std::shared_ptr<Value> value)
{
  if (!_root)
  {
    _root = new AVLNode(value);
    return _root;
  }

  AVLNode *current = _root;
  while (true)
  {
    if (*value < *current->val)
    {
      if (!current->_left)
      {
        current->_left = new AVLNode(value);
        current->_left->_parent = current;
        _root = current->fix();

        return current->_left;
      }

      current = current->_left;
    }
    else if (*value > *current->val)
    {
      if (!current->_right)
      {
        current->_right = new AVLNode(value);
        current->_right->_parent = current;
        _root = current->fix();

        return current->_right;
      }
      current = current->_right;
    }
    else
    {
      current->val = value;

      return current;
    }
  }
}

AVLNode *AVLTree::find(const std::shared_ptr<Value> &value) const
{
  AVLNode *current = _root;
  while (current)
  {
    if (*value < *current->val)
      current = current->_left;
    else if (*value > *current->val)
      current = current->_right;
    else
      return current;
  }

  return nullptr;
}

bool AVLTree::remove(const std::shared_ptr<Value> &value)
{
  AVLNode *node = find(value);
  if (!node)
    return false;

  _root = node->remove();

  node->_left = nullptr;
  node->_right = nullptr;
  node->_parent = nullptr;

  delete node;

  return true;
}

AVLNode *AVLTree::lowerBound(const std::shared_ptr<Value> &value) const
{
  AVLNode *current = _root;
  AVLNode *result = nullptr;
  while (current)
  {
    if (*current->val < *value)
      current = current->_right;
    else
    {
      result = current;
      current = current->_left;
    }
  }

  return result;
}

AVLNode *AVLTree::next(AVLNode *node) const
{
  if (!node)
    return nullptr;

  if (node->_right)
  {
    node = node->_right;
    while (node->_left)
      node = node->_left;

    return node;
  }

  while (node->_parent && node->_parent->_right == node)
    node = node->_parent;

  return node->_parent;
}

AVLNode *AVLTree::prev(AVLNode *node) const
{
  if (!node)
    return nullptr;

  if (node->_left)
  {
    node = node->_left;
    while (node->_right)
      node = node->_right;

    return node;
  }

  while (node->_parent && node->_parent->_left == node)
    node = node->_parent;

  return node->_parent;
}