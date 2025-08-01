#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class ByteBuffer
{
public:
  void append(const uint8_t *data, size_t len);
  void consume(size_t n);
  size_t size() const;
  const uint8_t *data() const;

private:
  std::vector<uint8_t> _buf;
};
