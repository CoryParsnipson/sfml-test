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
INCLUDE_PATHS = "SFML_Test:SFML_Test/Entities" 

CXX_FILES = 
CXX_FILES += $(PROJECT_DIR)/Util/FSM.cpp
CXX_FILES += $(PROJECT_DIR)/Entities/TestFSM.cpp

all: $(TARGET)

$(TARGET): $(PROJECT_DIR)/$(RUNNER_FILE)
	@echo ---- Making target: $(TARGET)
	@echo "  PROJECT_DIR: $(PROJECT_DIR)"
	@echo ""

	$(CC) $(CFLAGS) $(PROJECT_DIR)/$(RUNNER_FILE) $(CXX_FILES) -o $(TARGET) $(DL_INCLUDES) -I$(INCLUDE_PATHS) 
	
	chmod a+x $(TARGET)

clean:
	$(RM) $(TARGET)
