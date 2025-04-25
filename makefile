# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17 -Iinclude

# Target executable
TARGET = main

# Source and include directories
SRC_DIR = src
INC_DIR = include

# Collect all .cpp files in the src directory
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Object files (optional if you split compilation later)
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean up
clean:
	rm -f $(TARGET) $(OBJ)
