#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#define DEFAULT_HOST "0.0.0.0"
#define DEFAULT_PORT 8080

const size_t kMaxArg = 4096;

enum class ResponseTag : uint32_t
{
  TAG_NIL = 0, // nil
  TAG_ERR = 1, // error code + msg
  TAG_STR = 2, // string
  TAG_INT = 3, // int64
  TAG_DBL = 4, // double
  TAG_ARR = 5, // array
};

enum class ErrorCode : uint32_t
{
  ERR_UNKNOWN = 1,          // unknown command
  ERR_TOO_BIG = 2,          // response too big
  ERR_WRONG_ARGS_COUNT = 3, // wrong number of arguments
};

static void msg(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
}

static int32_t readFull(int fd, char *buf, size_t n)
{
  while (n > 0)
  {
    ssize_t rv = read(fd, buf, n);
    if (rv <= 0)
      return -1;

    assert((size_t)rv <= n);
    n -= (size_t)rv;
    buf += rv;
  }

  return 0;
}

static int32_t writeAll(int fd, const char *buf, size_t n)
{
  while (n > 0)
  {
    ssize_t rv = write(fd, buf, n);
    if (rv <= 0)
      return -1;

    assert((size_t)rv <= n);
    n -= (size_t)rv;
    buf += rv;
  }

  return 0;
}

static int32_t sendRequest(int fd, const std::vector<std::string> &cmd)
{
  uint32_t len = 4;
  for (const std::string &s : cmd)
    len += 4 + s.size();

  if (len > kMaxArg)
    return -1;

  char wbuf[4 + kMaxArg];
  memcpy(&wbuf[0], &len, 4);

  uint32_t n = (uint32_t)cmd.size();
  memcpy(&wbuf[4], &n, 4);

  size_t cur = 8;
  for (const std::string &s : cmd)
  {
    uint32_t p = (uint32_t)s.size();
    memcpy(&wbuf[cur], &p, 4);
    memcpy(&wbuf[cur + 4], s.data(), s.size());
    cur += 4 + s.size();
  }

  return writeAll(fd, wbuf, 4 + len);
}

static const char *getErrorName(uint32_t code)
{
  switch (code)
  {
  case (uint32_t)ErrorCode::ERR_UNKNOWN:
    return "ERR_UNKNOWN";
  case (uint32_t)ErrorCode::ERR_TOO_BIG:
    return "ERR_TOO_BIG";
  case (uint32_t)ErrorCode::ERR_WRONG_ARGS_COUNT:
    return "ERR_WRONG_ARGS_COUNT";
  default:
    return "UNKNOWN_ERROR";
  }
}

static int32_t printResponse(const uint8_t *data, size_t size)
{
  if (size < 1)
  {
    msg("bad response");
    return -1;
  }

  switch (data[0])
  {
  case (uint32_t)ResponseTag::TAG_NIL:
    printf("(nil)\n");
    return 1;
  case (uint32_t)ResponseTag::TAG_ERR:
    if (size < 1 + 8)
    {
      msg("bad response");
      return -1;
    }
    {
      uint32_t code = 0;
      uint32_t len = 0;
      memcpy(&code, &data[1], 4);
      memcpy(&len, &data[1 + 4], 4);
      if (size < 1 + 8 + len)
      {
        msg("bad response");
        return -1;
      }
      printf("(err) %s %.*s\n", getErrorName(code), len, &data[1 + 8]);

      return 1 + 8 + len;
    }
  case (uint32_t)ResponseTag::TAG_STR:
    if (size < 1 + 4)
    {
      msg("bad response");
      return -1;
    }
    {
      uint32_t len = 0;
      memcpy(&len, &data[1], 4);
      if (size < 1 + 4 + len)
      {
        msg("bad response");
        return -1;
      }
      printf("(str) %.*s\n", len, &data[1 + 4]);

      return 1 + 4 + len;
    }
  case (uint32_t)ResponseTag::TAG_INT:
    if (size < 1 + 8)
    {
      msg("bad response");
      return -1;
    }
    {
      int64_t val = 0;
      memcpy(&val, &data[1], 8);
      printf("(int) %ld\n", val);
      return 1 + 8;
    }
  case (uint32_t)ResponseTag::TAG_DBL:
    if (size < 1 + 8)
    {
      msg("bad response");
      return -1;
    }
    {
      double val = 0;
      memcpy(&val, &data[1], 8);
      printf("(dbl) %g\n", val);

      return 1 + 8;
    }
  case (uint32_t)ResponseTag::TAG_ARR:
    if (size < 1 + 4)
    {
      msg("bad response");
      return -1;
    }
    {
      uint32_t len = 0;
      memcpy(&len, &data[1], 4);
      printf("(arr) len=%u\n", len);
      size_t arr_bytes = 1 + 4;
      for (uint32_t i = 0; i < len; i++)
      {
        int32_t rv = printResponse(&data[arr_bytes], size - arr_bytes);
        if (rv < 0)
          return rv;

        arr_bytes += (size_t)rv;
      }
      printf("(arr) end\n");

      return (int32_t)arr_bytes;
    }
  default:
    msg("bad response");
    return -1;
  }
}

static int32_t read_res(int fd)
{
  char buf[4 + kMaxArg + 1];
  errno = 0;
  int32_t err = readFull(fd, buf, 4);
  if (err)
  {
    if (errno == 0)
      msg("EOF");
    else
      msg("read() error");

    return err;
  }

  uint32_t len = 0;
  memcpy(&len, buf, 4);
  if (len > kMaxArg)
  {
    msg("too long");
    return -1;
  }

  err = readFull(fd, &buf[4], len);
  if (err)
  {
    msg("read() error");
    return err;
  }

  int32_t rv = printResponse((uint8_t *)&buf[4], len);
  if (rv > 0 && (uint32_t)rv != len)
  {
    msg("bad response");
    rv = -1;
  }

  return rv;
}

int main()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
  {
    std::cout << "Failed to create socket" << std::endl;
    return -1;
  }

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(DEFAULT_PORT);
  addr.sin_addr.s_addr = inet_addr(DEFAULT_HOST);
  int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv)
  {
    close(fd);
    std::cout << "Failed to connect to server" << std::endl;
    return -1;
  }

  std::cout << "Connected to server\n";
  std::cout << "redis> " << std::flush;

  std::string line;
  while (true)
  {
    if (!std::getline(std::cin, line))
      break;

    if (line == "quit" || line == "exit")
      break;

    if (line.empty())
    {
      std::cout << "redis> " << std::flush;
      continue;
    }

    std::vector<std::string> cmd;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token)
      cmd.push_back(token);

    if (cmd.empty())
    {
      std::cout << "redis> " << std::flush;
      continue;
    }

    int32_t err = sendRequest(fd, cmd);
    if (err)
    {
      std::cout << "Failed to send request" << std::endl;
      break;
    }

    err = read_res(fd);
    if (err < 0)
    {
      std::cout << "Failed to read response" << std::endl;
      break;
    }

    std::cout << "redis> " << std::flush;
  }

  close(fd);
  return 0;
}