# Redis Clone

A lightweight Redis server clone implemented in C++ that supports key-value operations and sorted sets.

## Overview 📘

This project is a simplified implementation of Redis, featuring:

-   In-memory key-value storage
-   Binary protocol for client-server communication
-   Support for strings, integers, and sorted sets
-   Command processing similar to Redis
-   Non-blocking I/O with event loop architecture

## Building 🛠️

### Prerequisites

-   C++17 compatible compiler (g++ recommended)
-   Linux/Unix environment

### Compilation

```bash
# Build server and client
make
```

## Running ▶️

### Server

```bash
./redis-clone
```

The server listens on 0.0.0.0:8080 by default.

### Client

```bash
./client
```

## Supported Commands 🧾

The server supports the following Redis-like commands:

### Basic Commands

-   `GET key` - Get the value of a key
-   `SET key value` - Set key to value
-   `DEL key` - Delete key
-   `KEYS` - Get all keys
-   `PTTL key` - Get the time to live for a key in milliseconds
-   `PEXPIRE key milliseconds` - Set a key's time-to-live in milliseconds

### Sorted Set Commands

-   `ZADD key score member` - Add member to a sorted set
-   `ZREM key member` - Remove member from a sorted set
-   `ZSCORE key member` - Get the score of a member in a sorted set
-   `ZQUERY key min max` - Query members within a score range

### Usage Examples

```
SET mykey hello
GET mykey
PTTL mykey
PEXPIRE mykey 1500
PTTL mykey
DEL mykey
ZADD ranks 10 alice
ZADD ranks 5 bob
ZSCORE ranks alice
ZQUERY ranks 0 10
```

## Protocol Details 📡

#### Request Format

```
msglen (4 bytes) | nargs (4 bytes) | arglen1 (4 bytes) | arg1 | ... | arglenN (4 bytes) | argN
```

Where:

-   msglen: Total message length (excluding the 4 bytes for msglen itself)
-   nargs: Number of arguments
-   arglenX: Length of argument X
-   argX: Byte data for argument X

#### Response Format

```
msglen (4 bytes) | tag (1 byte) | payload
```

Where:

-   msglen: Total message length (excluding the 4 bytes for msglen itself)
-   tag: Type of response (NIL, ERROR, STRING, INTEGER, DOUBLE, ARRAY)
-   payload: Type-specific data format

### Example Encoded Request (SET key value)

Arguments: ["SET","mykey","hello"]

```
[msglen= ?] | nargs=3
arglen=3  SET
arglen=5  mykey
arglen=5  hello
```

Hex representation in little-endian:

```
18 00 00 00    # msglen = 24 (bytes after this field)
03 00 00 00    # nargs
03 00 00 00 53 45 54
05 00 00 00 6d 79 6b 65 79
05 00 00 00 68 65 6c 6c 6f
```

## Architecture 🧱

### Core Components

-   **Server**: Main server implementation handling connections
-   **EventLoop**: Non-blocking I/O event loop using epoll()
-   **ConnectionManager**: Manages client connections
-   **RequestProcessor**: Processes incoming client requests
-   **ProtocolHelper**: Handles the binary protocol encoding/decoding
-   **Command System**: Extensible command pattern implementation
-   **Database**: In-memory data storage with HashTable and AVL tree

### Data Structures

-   **ResizableHashTable**: Main key-value storage
-   **AVLTree**: Self-balancing binary tree for sorted sets

### Core Components

1. **Server**:

    - Central component that initializes and coordinates all other subsystems
    - Manages the lifecycle of the application
    - Handles signal processing and graceful shutdown

2. **Event Loop**:

    - Non-blocking I/O multiplexer using `epoll()`
    - Efficiently handles multiple client connections concurrently
    - Event-based architecture that processes read/write events as they occur
    - Avoids thread-per-connection overhead while maintaining responsiveness

3. **Connection Management**:

    - Manages client connection lifecycles
    - Handles socket I/O operations through the `Connection` class
    - Tracks connection state (read/write/closed)
    - Implements connection timeouts to prevent resource leaks
    - Uses `ByteBuffer` for efficient input/output buffering

4. **Protocol Layer**:

    - Binary protocol implementation for efficient communication
    - `ProtocolHelper` encapsulates serialization/deserialization logic
    - Support for different data types (strings, integers, doubles, arrays)
    - Request parsing and response formatting
    - Error handling and protocol validation

5. **Command Processing Pipeline**:

    - `RequestProcessor` receives and routes client requests
    - Command pattern implementation for extensibility
    - `CommandFactory` creates appropriate command handlers
    - Each command implemented as a separate class with consistent interface
    - Error handling and response generation

6. **Storage Engine**:
    - In-memory data structures optimized for different access patterns
    - `ResizableHashTable` as the main key-value store with dynamic resizing
    - `AVLTree` for sorted set implementation with O(log n) operations
    - Value types system for different data representations
    - Expiration mechanism for key TTL implementation

### Data Flow

1. Client connection is accepted by the Server
2. EventLoop monitors socket for read/write readiness
3. When data is available, Connection.handleRead() buffers the incoming data
4. RequestProcessor attempts to parse a complete request using ProtocolHelper
5. If a complete request is available, CommandFactory creates the appropriate command
6. Command is executed against the database, producing a Response
7. Response is serialized by ProtocolHelper and written to the connection's output buffer
8. EventLoop detects write readiness and Connection.handleWrite() sends the response
9. After sending, connection returns to read mode waiting for the next request

### Design Patterns

-   **Command Pattern**: All operations encapsulated as Command objects with uniform interface
-   **Factory Pattern**: CommandFactory creates appropriate Command instances
-   **Repository Pattern**: IDatabase interface abstracts storage operations
-   **Strategy Pattern**: Different command implementations for various operations

## Attribution 📖

This implementation is based on (and adapted from) the concepts and structure presented in the book:
Build Your Own Redis with C/C++  
Network programming, data structures, and low-level C.  
Author: James Smith  
https://build-your-own.org/redis/

The code here is an independent reimplementation inspired by that guide.

## License 📄

This project is open source and available under the MIT License. See LICENSE file.
