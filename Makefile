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

INCLUDE_PATHS += $(PROJECT_DIR)/vendor
INCLUDE_PATHS += $(PROJECT_DIR)/vendor/json

INCLUDE_PATHS += $(PROJECT_DIR)
INCLUDE_PATHS += $(PROJECT_DIR)/engine
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Util
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Logger
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Player
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Game
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input/InputEvents
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input/InputDevices
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input/Intents
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input/Gamepad
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input/GamepadPreset
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Input
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Graphics
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Command
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Command/Mouse
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Scene
INCLUDE_PATHS += $(PROJECT_DIR)/engine/ECS/Messaging
INCLUDE_PATHS += $(PROJECT_DIR)/engine/ECS/Messaging/Message
INCLUDE_PATHS += $(PROJECT_DIR)/engine/ECS/Component
INCLUDE_PATHS += $(PROJECT_DIR)/engine/ECS/Entity
INCLUDE_PATHS += $(PROJECT_DIR)/engine/ECS/System
INCLUDE_PATHS += $(PROJECT_DIR)/engine/ECS/System/EntitySubscription
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Channel
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Serializer
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Map
INCLUDE_PATHS += $(PROJECT_DIR)/engine/Factory
INCLUDE_PATHS += $(PROJECT_DIR)/engine/UI/Widget
INCLUDE_PATHS += $(PROJECT_DIR)/engine/UI/Compositor
INCLUDE_PATHS += $(PROJECT_DIR)/engine/UI

INCLUDE_PATHS += $(PROJECT_DIR)/levels
INCLUDE_PATHS += $(PROJECT_DIR)/levels/StartMenuSceneECS
INCLUDE_PATHS += $(PROJECT_DIR)/levels/StartMenuSceneECS/System
INCLUDE_PATHS += $(PROJECT_DIR)/levels/BuilderSceneECS
INCLUDE_PATHS += $(PROJECT_DIR)/levels/BuilderSceneECS/Component
INCLUDE_PATHS += $(PROJECT_DIR)/levels/BuilderSceneECS/Intent
INCLUDE_PATHS += $(PROJECT_DIR)/levels/BuilderSceneECS/Message
INCLUDE_PATHS += $(PROJECT_DIR)/levels/BuilderSceneECS/System

CXX_FILES =
CXX_FILES += $(PROJECT_DIR)/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Logger/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Player/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Game/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/InputEvents/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/InputDevices/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/Gamepad/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/GamepadPreset/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Input/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Graphics/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Scene/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Command/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Command/Mouse/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/ECS/Messaging/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/ECS/Component/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/ECS/Entity/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/ECS/System/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/ECS/System/EntitySubscription/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Channel/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Serializer/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Map/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/Factory/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/UI/Widget/*.cpp
CXX_FILES += $(PROJECT_DIR)/engine/UI/Compositor/*.cpp

CXX_FILES += $(PROJECT_DIR)/levels/StartMenuSceneECS/*.cpp
CXX_FILES += $(PROJECT_DIR)/levels/StartMenuSceneECS/System/*.cpp
CXX_FILES += $(PROJECT_DIR)/levels/BuilderSceneECS/*.cpp
CXX_FILES += $(PROJECT_DIR)/levels/BuilderSceneECS/Component/*.cpp
CXX_FILES += $(PROJECT_DIR)/levels/BuilderSceneECS/System/*.cpp

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
	$(RM) $(OUTPUT_DIR_PROFILER)/$(TARGET)
