#pragma once

class Value
{
public:
  virtual ~Value() = default;

  virtual bool operator<(const Value &other) const = 0;
  virtual bool operator>(const Value &other) const = 0;
  virtual bool operator==(const Value &other) const = 0;
};
