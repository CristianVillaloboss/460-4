# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Target executable
TARGET = main

# Source files
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Clean rule
clean:
	rm -f $(TARGET) *.o