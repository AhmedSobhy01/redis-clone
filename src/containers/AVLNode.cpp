#include "containers/AVLNode.h"

AVLNode::AVLNode(std::shared_ptr<Value> value) : _parent(nullptr), _left(nullptr), _right(nullptr), _height(1), _count(1), val(std::move(value))
{
}

AVLNode::~AVLNode()
{
  delete _left;
  delete _right;
}

void AVLNode::update()
{
  _height = 1 + std::max(getHeight(_left), getHeight(_right));
  _count = 1 + getCount(_left) + getCount(_right);
}

AVLNode *AVLNode::rotateLeft()
{
  AVLNode *parent = _parent;
  AVLNode *newNode = _right;
  AVLNode *inner = newNode->_left;

  // this <-> inner
  _right = inner;
  if (inner)
    inner->_parent = this;

  // parent <- newNode
  newNode->_parent = parent;

  // newNode <-> this
  newNode->_left = this;
  _parent = newNode;

  update();
  newNode->update();

  return newNode;
}

AVLNode *AVLNode::rotateRight()
{
  AVLNode *parent = _parent;
  AVLNode *newNode = _left;
  AVLNode *inner = newNode->_right;

  // this <-> inner
  _left = inner;
  if (inner)
    inner->_parent = this;

  // parent <- newNode
  newNode->_parent = parent;

  // newNode <-> this
  newNode->_right = this;
  _parent = newNode;

  update();
  newNode->update();

  return newNode;
}

AVLNode *AVLNode::fixLeft()
{
  if (getHeight(_left->_left) < getHeight(_left->_right))
    _left = _left->rotateLeft();

  return rotateRight();
}

AVLNode *AVLNode::fixRight()
{
  if (getHeight(_right->_right) < getHeight(_right->_left))
    _right = _right->rotateRight();

  return rotateLeft();
}

AVLNode *AVLNode::fix()
{
  AVLNode *node = this;

  while (true)
  {
    AVLNode **from = &node;
    AVLNode *parent = node->_parent;

    if (parent)
      from = parent->_left == node ? &parent->_left : &parent->_right;

    node->update();

    // fix the height difference of 2
    uint32_t l = getHeight(node->_left);
    uint32_t r = getHeight(node->_right);

    if (l == r + 2)
      *from = node->fixLeft();
    else if (l + 2 == r)
      *from = node->fixRight();

    // root node
    if (!parent)
      return *from;

    // continue to the parent node
    node = parent;
  }
}

AVLNode *AVLNode::removeWithOneOrNoneChild()
{
  AVLNode *child = _left ? _left : _right;
  AVLNode *parent = _parent;

  // child's parent
  if (child)
    child->_parent = parent;

  // if we are the root
  if (!parent)
    return child;

  AVLNode **from = parent->_left == this ? &parent->_left : &parent->_right;
  *from = child;

  return parent->fix();
}

AVLNode *AVLNode::remove()
{
  if (!_left || !_right)
    return removeWithOneOrNoneChild();

  // find the successor
  AVLNode *victim = _right;
  while (victim->_left)
    victim = victim->_left;

  AVLNode *root = victim->removeWithOneOrNoneChild();

  // swap with the successor
  victim->_left = _left;
  victim->_right = _right;
  victim->_parent = _parent;

  if (victim->_left)
    victim->_left->_parent = victim;
  if (victim->_right)
    victim->_right->_parent = victim;

  // attach the successor to the parent
  AVLNode **from = &root;
  AVLNode *parent = _parent;
  if (parent)
    from = parent->_left == this ? &parent->_left : &parent->_right;
  *from = victim;

  return root;
}
