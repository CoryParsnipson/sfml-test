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
OUTPUT_DIR_PROFILER := $(OUTPUT_DIR)/Profiler
OUTPUT_DIR_DEBUG := $(OUTPUT_DIR)/Debug
OUTPUT_DIR_RELEASE := $(OUTPUT_DIR)/Release

# dll stuff
DL_INCLUDES = -lsfml-window -lsfml-graphics -lsfml-system

# include paths
INCLUDE_PATHS = 
INCLUDE_PATHS += SFML_Test
INCLUDE_PATHS += SFML_Test/Util
INCLUDE_PATHS += SFML_Test/Input/InputEvent
INCLUDE_PATHS += SFML_Test/Input/Gamepad
INCLUDE_PATHS += SFML_Test/Input
INCLUDE_PATHS += SFML_Test/Logger
INCLUDE_PATHS += SFML_Test/Service
INCLUDE_PATHS += SFML_Test/Game
INCLUDE_PATHS += SFML_Test/Graphics
INCLUDE_PATHS += SFML_Test/Command
INCLUDE_PATHS += SFML_Test/Scene
INCLUDE_PATHS += SFML_Test/Entity
INCLUDE_PATHS += SFML_Test/Builder
INCLUDE_PATHS += SFML_Test/Serializer
INCLUDE_PATHS += SFML_Test/Map
INCLUDE_PATHS += SFML_Test/Manager
INCLUDE_PATHS += SFML_Test/Factory
INCLUDE_PATHS += SFML_Test/UI/Widget
INCLUDE_PATHS += SFML_Test/UI/Compositor
INCLUDE_PATHS += SFML_Test/UI

CXX_FILES =
CXX_FILES += $(PROJECT_DIR)/*.cpp
CXX_FILES += $(PROJECT_DIR)/Input/InputEvent/*.cpp
CXX_FILES += $(PROJECT_DIR)/Input/Gamepad/*.cpp
CXX_FILES += $(PROJECT_DIR)/Input/*.cpp
CXX_FILES += $(PROJECT_DIR)/Logger/*.cpp
CXX_FILES += $(PROJECT_DIR)/Service/*.cpp
CXX_FILES += $(PROJECT_DIR)/Game/*.cpp
CXX_FILES += $(PROJECT_DIR)/Graphics/*.cpp
CXX_FILES += $(PROJECT_DIR)/Scene/*.cpp
CXX_FILES += $(PROJECT_DIR)/Command/*.cpp
CXX_FILES += $(PROJECT_DIR)/Entity/*.cpp
CXX_FILES += $(PROJECT_DIR)/Builder/*.cpp
CXX_FILES += $(PROJECT_DIR)/Serializer/*.cpp
CXX_FILES += $(PROJECT_DIR)/Map/*.cpp
CXX_FILES += $(PROJECT_DIR)/Manager/*.cpp
CXX_FILES += $(PROJECT_DIR)/Factory/*.cpp
CXX_FILES += $(PROJECT_DIR)/UI/Widget/*.cpp
CXX_FILES += $(PROJECT_DIR)/UI/Compositor/*.cpp

.PHONY: profiler debug release
.DEFAULT_GOAL := debug

profiler: CFLAGS += -pg
profiler: OUTPUT_DIR = $(OUTPUT_DIR_PROFILER)

debug: OUTPUT_DIR = $(OUTPUT_DIR_DEBUG)

release: CFLAGS += -O3
release: OUTPUT_DIR = $(OUTPUT_DIR_RELEASE)

all profiler debug release: $(PROJECT_DIR)/$(RUNNER_FILE)
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
