#pragma once

#include <cstdint>
#include <vector>

enum class ResponseStatus : uint32_t
{
  OK = 1,
  NOT_FOUND = 2,
  ERROR = 3
};

struct Response
{
  ResponseStatus status;
  std::vector<uint8_t> data;
};
