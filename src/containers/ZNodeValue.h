#pragma once

#include "containers/Value.h"
#include "containers/AVLTree.h"
#include "containers/ResizableHashTable.h"

class ZNodeValue : public Value
{
private:
  std::string _name;
  double _score;

public:
  ZNodeValue(const std::string &name, double score) : _name(name), _score(score) {}

  const std::string &name() const { return _name; }
  double score() const { return _score; }
  void setScore(double s) { _score = s; }

  bool operator<(const Value &other) const override
  {
    const ZNodeValue *otherNode = dynamic_cast<const ZNodeValue *>(&other);
    if (!otherNode)
      return false;

    if (_score == otherNode->_score)
      return _name < otherNode->_name;

    return _score < otherNode->_score;
  }

  bool operator>(const Value &other) const override
  {
    const ZNodeValue *otherNode = dynamic_cast<const ZNodeValue *>(&other);
    if (!otherNode)
      return false;

    if (_score == otherNode->_score)
      return _name > otherNode->_name;

    return _score > otherNode->_score;
  }

  bool operator==(const Value &other) const override
  {
    const ZNodeValue *otherNode = dynamic_cast<const ZNodeValue *>(&other);
    if (!otherNode)
      return false;

    return _score == otherNode->_score && _name == otherNode->_name;
  }
};
