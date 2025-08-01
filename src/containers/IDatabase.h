#pragma once

#include <string>

class IDatabase
{
public:
  virtual ~IDatabase() = default;
  virtual bool get(const std::string &key, std::string &out) = 0;
  virtual void set(const std::string &key, std::string value) = 0;
  virtual bool del(const std::string &key) = 0;
  virtual size_t size() const = 0;
  virtual void clear() = 0;
};
