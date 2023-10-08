# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17

# Source files and object files
SRCS := $(wildcard *.cpp) $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

# Dependency files
DEPS := $(OBJS:.o=.d)

# Executable name
EXECUTABLE := main.exe

# SFML settings
SFML_PATH := SFML

# Include and library directories
INCLUDE_DIRS := -I$(SFML_PATH)/include
LIB_DIRS := -L$(SFML_PATH)/lib
INCLUDE_HASHMAP := ../src/hash_map.c

# Libraries to link against
SFML_LIBRARIES := -lsfml-graphics -lsfml-window -lsfml-system

# Windows-specific settings
RM := del /Q

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(LIB_DIRS) $(INCLUDE_HASHMAP) $^ -o $@ $(SFML_LIBRARIES)

# Include dependency files
-include $(DEPS)

# Generate dependency files
%.d: %.cpp
	@$(CXX) $(CXXFLAGS) -MM -MT "$@ $(@:.d=.o)" $< -MF $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(DEPS) $(EXECUTABLE)
