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
PROJECT_DIR := $(PWD)
OUTPUT_DIR := $(PWD)/output
OUTPUT_DIR_PROFILER := $(OUTPUT_DIR)/profiler
OUTPUT_DIR_DEBUG := $(OUTPUT_DIR)/debug
OUTPUT_DIR_RELEASE := $(OUTPUT_DIR)/release

# dll stuff
DL_INCLUDES = -lsfml-window -lsfml-graphics -lsfml-system

# include paths
INCLUDE_PATHS = 
INCLUDE_PATHS += $(PROJECT_DIR)
INCLUDE_PATHS += $(PROJECT_DIR)/engine
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Util
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input/InputEvent
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input/Gamepad
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input/GamepadPreset
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Logger
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Service
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Game
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Graphics
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Command
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Command/Mouse
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Scene
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Entity
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Builder
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Serializer
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Map
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Factory
INCLUDE_PATHS += $(PROJECT_DIR)/engine/UI/Widget
INCLUDE_PATHS += $(PROJECT_DIR)/engine/UI/Compositor
INCLUDE_PATHS += $(PROJECT_DIR)/engine/UI

INCLUDE_PATHS += $(PROJECT_DIR)/levels
INCLUDE_PATHS += $(PROJECT_DIR)/levels/StartMenuScene
INCLUDE_PATHS += $(PROJECT_DIR)/levels/BuilderScene
INCLUDE_PATHS += $(PROJECT_DIR)/levels/BuilderScene/Commands
INCLUDE_PATHS += $(PROJECT_DIR)/levels/TestUIScene

CXX_FILES =
CXX_FILES += $(PROJECT_DIR)/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/InputEvent/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/Gamepad/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/GamepadPreset/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Logger/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Service/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Game/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Graphics/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Scene/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Command/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Command/Mouse/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Entity/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Builder/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Serializer/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Map/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Factory/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/UI/Widget/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/UI/Compositor/*.cpp

CXX_FILES += $(PROJECT_DIR)/levels/StartMenuScene/*.cpp
CXX_FILES += $(PROJECT_DIR)/levels/BuilderScene/*.cpp
CXX_FILES += $(PROJECT_DIR)/levels/BuilderScene/Commands/*.cpp
CXX_FILES += $(PROJECT_DIR)/levels/TestUIScene/*.cpp

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
