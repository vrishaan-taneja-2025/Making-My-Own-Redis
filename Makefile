# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -IRedis-Client/include

# Directories
SRC_DIR = Redis-Client/src
INC_DIR = Redis-Client/include
BUILD_DIR = build
BIN_DIR = bin

# Find all .cpp files in src and map them to .o files in build
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Output binary
TARGET = $(BIN_DIR)/my_redis_cli

# Default rule
all: $(TARGET)

# Create build and bin directories if they don't exist
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

# Compile each .cpp file into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Rebuild everything
rebuild: clean all

# Run the compiled binary
run: all
	./$(TARGET)
