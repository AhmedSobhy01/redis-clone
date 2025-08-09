#pragma once

#include "containers/Value.h"
#include "containers/AVLTree.h"
#include "containers/ResizableHashTable.h"
#include "containers/ZNodeValue.h"

#include <vector>
#include <utility>
#include <cstdint>

class ZSetValue : public Value
{
private:
  AVLTree _tree;
  ResizableHashTable _hashtable;

public:
  bool add(const std::string &name, double score)
  {
    std::shared_ptr<ZNodeValue> old = std::dynamic_pointer_cast<ZNodeValue>(_hashtable.get(name));

    if (old)
    {
      _tree.remove(old);
      old->setScore(score);
      _tree.insert(old);

      return false;
    }

    std::shared_ptr<ZNodeValue> node = std::make_shared<ZNodeValue>(name, score);
    _hashtable.set(name, node);
    _tree.insert(node);

    return true;
  }

  bool remove(const std::string &name)
  {
    std::shared_ptr<ZNodeValue> old = std::dynamic_pointer_cast<ZNodeValue>(_hashtable.get(name));

    if (!old)
      return false;

    _tree.remove(old);
    _hashtable.del(name);

    return true;
  }

  bool score(const std::string &name, double &out)
  {
    std::shared_ptr<ZNodeValue> old = std::dynamic_pointer_cast<ZNodeValue>(_hashtable.get(name));

    if (!old)
      return false;

    out = old->score();
    return true;
  }

  void query(double score, const std::string &name, int64_t offset, int64_t limit, std::vector<std::pair<std::string, double>> &res) const
  {
    if (limit <= 0)
      return;

    std::shared_ptr<ZNodeValue> key = std::make_shared<ZNodeValue>(name, score);
    AVLNode *node = _tree.lowerBound(key);

    // apply offset
    for (int64_t i = 0; i < offset && node; i++)
      node = _tree.next(node);

    // get nodes
    while (node && (int64_t)res.size() < limit)
    {
      std::shared_ptr<ZNodeValue> zv = std::dynamic_pointer_cast<ZNodeValue>(node->value());
      res.emplace_back(zv->name(), zv->score());
      node = _tree.next(node);
    }
  }

  // dummy overloads since not used anywhere
  bool operator<(const Value &) const override { return false; }
  bool operator>(const Value &) const override { return false; }
  bool operator==(const Value &other) const override { return this == &other; }
};
