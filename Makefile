# Makefile
#
# author: Cory Parsnipson, 2015
#

# makefile defines
CC = g++
TARGET = sfml_test

# compiler flags:
# -g         adds debugging information to the executable file
# -Wall      turns on most, but not all, compiler warnings
# -std=c++11 adhere to the "experimental" C++11 ISO standard
CFLAGS = -g -Wall -std=c++11

# path defines
PROJECT_DIR := $(PWD)/SFML_Test
OUTPUT_DIR := $(PWD)
OUTPUT_DIR_DEBUG := $(PWD)/Debug
OUTPUT_DIR_RELEASE := $(PWD)/Release

# dll stuff
DL_INCLUDES = -lsfml-window -lsfml-graphics -lsfml-system

# include paths
INCLUDE_PATHS = SFML_Test SFML_Test/Entities

CXX_FILES = 
CXX_FILES += $(PROJECT_DIR)/*.cpp
CXX_FILES += $(PROJECT_DIR)/Util/*.cpp
CXX_FILES += $(PROJECT_DIR)/Entities/*.cpp
CXX_FILES += $(PROJECT_DIR)/FileReader/*.cpp

.PHONY: for_debug for_release
.DEFAULT_GOAL := for_debug

for_debug: OUTPUT_DIR = $(OUTPUT_DIR_DEBUG)
for_release: OUTPUT_DIR = $(OUTPUT_DIR_RELEASE)

all for_debug for_release: $(PROJECT_DIR)/$(RUNNER_FILE)
	@echo ---- Making target: $(TARGET)
	@echo "  PROJECT_DIR : $(PROJECT_DIR)"
	@echo "  TARGET_DIR  : $(OUTPUT_DIR)"
	@echo ""

	$(CC) $(CFLAGS) $(CXX_FILES) -o $(TARGET) $(addprefix -I, $(INCLUDE_PATHS)) $(DL_INCLUDES)
	
	chmod a+x $(TARGET) 
	mkdir -p $(OUTPUT_DIR)
	mv $(TARGET) $(OUTPUT_DIR)

clean:
	$(RM) $(TARGET)
	$(RM) $(OUTPUT_DIR_DEBUG)/$(TARGET)
	$(RM) $(OUTPUT_DIR_RELEASE)/$(TARGET)
