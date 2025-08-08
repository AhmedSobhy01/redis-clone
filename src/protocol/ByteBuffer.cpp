#include "protocol/ByteBuffer.h"

#include <algorithm>

void ByteBuffer::append(const uint8_t *data, size_t len)
{
  _buf.insert(_buf.end(), data, data + len);
}

void ByteBuffer::consume(size_t n)
{
  if (n >= _buf.size())
    _buf.clear();
  else
    _buf.erase(_buf.begin(), _buf.begin() + n);
}

size_t ByteBuffer::size() const
{
  return _buf.size();
}

const uint8_t *ByteBuffer::data() const
{
  return _buf.empty() ? nullptr : _buf.data();
}