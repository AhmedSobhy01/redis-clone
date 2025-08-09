CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
SRCDIR = src
OBJDIR = obj
TARGET = redis-clone

# Find all source files
SOURCES = $(shell find $(SRCDIR) -name "*.cpp")
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Include directories
INCLUDES = -I$(SRCDIR)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

# Dependencies
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/server/Server.h $(SRCDIR)/containers/ResizableHashTable.h
$(OBJDIR)/server/Server.o: $(SRCDIR)/server/Server.cpp $(SRCDIR)/server/Server.h $(SRCDIR)/network/EventLoop.h $(SRCDIR)/network/ConnectionManager.h $(SRCDIR)/containers/IDatabase.h $(SRCDIR)/server/RequestProcessor.h
$(OBJDIR)/server/RequestProcessor.o: $(SRCDIR)/server/RequestProcessor.cpp $(SRCDIR)/server/RequestProcessor.h $(SRCDIR)/containers/IDatabase.h $(SRCDIR)/network/Connection.h $(SRCDIR)/protocol/ProtocolHelper.h $(SRCDIR)/command/CommandFactory.h
$(OBJDIR)/containers/ResizableHashTable.o: $(SRCDIR)/containers/ResizableHashTable.cpp $(SRCDIR)/containers/ResizableHashTable.h $(SRCDIR)/containers/HashTable.h $(SRCDIR)/containers/IDatabase.h
$(OBJDIR)/containers/HashTable.o: $(SRCDIR)/containers/HashTable.cpp $(SRCDIR)/containers/HashTable.h $(SRCDIR)/containers/IDatabase.h
$(OBJDIR)/network/EventLoop.o: $(SRCDIR)/network/EventLoop.cpp $(SRCDIR)/network/EventLoop.h $(SRCDIR)/network/ConnectionManager.h $(SRCDIR)/network/Listener.h
$(OBJDIR)/network/ConnectionManager.o: $(SRCDIR)/network/ConnectionManager.cpp $(SRCDIR)/network/ConnectionManager.h $(SRCDIR)/network/Connection.h
$(OBJDIR)/network/Connection.o: $(SRCDIR)/network/Connection.cpp $(SRCDIR)/network/Connection.h $(SRCDIR)/protocol/ByteBuffer.h
$(OBJDIR)/network/Listener.o: $(SRCDIR)/network/Listener.cpp $(SRCDIR)/network/Listener.h $(SRCDIR)/network/ConnectionManager.h
$(OBJDIR)/protocol/ProtocolHelper.o: $(SRCDIR)/protocol/ProtocolHelper.cpp $(SRCDIR)/protocol/ProtocolHelper.h $(SRCDIR)/protocol/ByteBuffer.h $(SRCDIR)/protocol/Request.h $(SRCDIR)/protocol/Response.h
$(OBJDIR)/protocol/Response.o: $(SRCDIR)/protocol/Response.cpp $(SRCDIR)/protocol/Response.h
$(OBJDIR)/protocol/ByteBuffer.o: $(SRCDIR)/protocol/ByteBuffer.cpp $(SRCDIR)/protocol/ByteBuffer.h
$(OBJDIR)/command/CommandFactory.o: $(SRCDIR)/command/CommandFactory.cpp $(SRCDIR)/command/CommandFactory.h $(SRCDIR)/command/GetCommand.h $(SRCDIR)/command/SetCommand.h $(SRCDIR)/command/DelCommand.h
$(OBJDIR)/command/GetCommand.o: $(SRCDIR)/command/GetCommand.cpp $(SRCDIR)/command/GetCommand.h $(SRCDIR)/command/Command.h
$(OBJDIR)/command/SetCommand.o: $(SRCDIR)/command/SetCommand.cpp $(SRCDIR)/command/SetCommand.h $(SRCDIR)/command/Command.h
$(OBJDIR)/command/DelCommand.o: $(SRCDIR)/command/DelCommand.cpp $(SRCDIR)/command/DelCommand.h $(SRCDIR)/command/Command.h
