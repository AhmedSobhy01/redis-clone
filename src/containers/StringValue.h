#pragma once

#include "containers/Value.h"

class StringValue : public Value
{
private:
  std::string _value;

public:
  StringValue(const std::string &value) : _value(value) {}

  const std::string &getValue() const { return _value; }
  void setValue(const std::string &value) { _value = value; }

  bool operator<(const Value &other) const override
  {
    const StringValue *otherString = dynamic_cast<const StringValue *>(&other);
    if (!otherString)
      return false;

    return _value < otherString->_value;
  }

  bool operator>(const Value &other) const override
  {
    const StringValue *otherString = dynamic_cast<const StringValue *>(&other);
    if (!otherString)
      return false;

    return _value > otherString->_value;
  }

  bool operator==(const Value &other) const override
  {
    const StringValue *otherString = dynamic_cast<const StringValue *>(&other);
    if (!otherString)
      return false;

    return _value == otherString->_value;
  }
};
