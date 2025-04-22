# Compiler
CXX = g++

# Compiler flags (optional: -Wall shows all warnings)
CXXFLAGS = -Wall -std=c++17

# Target executable name
TARGET = main

# Source files
SRC = main.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean up the compiled files
clean:
	rm -f $(TARGET)
