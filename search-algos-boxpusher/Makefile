CC        := g++
LD        := g++

MODULES   := analytics enums game hash io timer util
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))
MAIN_FILE := src/main.cpp

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs bin/game.exe

bin/game.exe: $(OBJ)
	$(LD) $^  $(MAIN_FILE) -o $@ $(INCLUDES)


checkdirs: $(BUILD_DIR) bin

$(BUILD_DIR):
	@mkdir -p $@

bin:
	@mkdir -p bin

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))