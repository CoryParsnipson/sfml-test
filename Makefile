# the compiler
CC = g++

# compiler flags:
# -g         adds debugging information to the executable file
# -Wall      turns on most, but not all, compiler warnings
CFLAGS = -g -Wall -std=c++11

# path defines
PROJECT_DIR := $(PWD)/SFML_Test
SFML_DIR := $(PWD)/SFML
SFML_INCLUDE_DIR := $(SFML_DIR)/include
SFML_LIBRARY_DIR := $(SFML_DIR)/lib

# main defines
TARGET = main 
RUNNER_FILE := main.cpp

all: $(TARGET)

$(TARGET): $(PROJECT_DIR)/$(RUNNER_FILE)
	@echo ---- Making target: $(TARGET)
	@echo "  PROJECT_DIR: $(PROJECT_DIR)"
	@echo "  SFML_DIR: $(SFML_DIR)"
	@echo "  SFML_INCLUDE_DIR: $(SFML_INCLUDE_DIR)"
	@echo "  SFML_LIBRARY_DIR: $(SFML_LIBRARY_DIR)"
	@echo ""

	$(CC) $(CFLAGS) -o $(TARGET) -c $(PROJECT_DIR)/$(RUNNER_FILE) -L$(SFML_LIBRARY_DIR) -I$(SFML_INCLUDE_DIR)
	
	chmod a+x $(TARGET)

clean:
	$(RM) $(TARGET)
