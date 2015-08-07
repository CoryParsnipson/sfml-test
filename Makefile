# the compiler
CC = g++

# compiler flags:
# -g         adds debugging information to the executable file
# -Wall      turns on most, but not all, compiler warnings
# -std=c++11 adhere to the "experimental" C++11 ISO standard
CFLAGS = -g -Wall -std=c++11

# path defines
PROJECT_DIR := $(PWD)/SFML_Test

# main defines
TARGET = main 
RUNNER_FILE := main.cpp

DL_INCLUDES = -lsfml-window -lsfml-graphics -lsfml-system

all: $(TARGET)

$(TARGET): $(PROJECT_DIR)/$(RUNNER_FILE)
	@echo ---- Making target: $(TARGET)
	@echo "  PROJECT_DIR: $(PROJECT_DIR)"
	@echo ""

	$(CC) $(CFLAGS) -o $(TARGET) $(PROJECT_DIR)/$(RUNNER_FILE) $(DL_INCLUDES) 
	
	chmod a+x $(TARGET)

clean:
	$(RM) $(TARGET)
