#
# General Makefile
#

BUILD := build
INC := -Iinclude
CXXFLAG := -std=c++20 $(INC)
CXX := g++
SRC := $(shell find . -name '*.cc')
OBJ := $(SRC:%.cc=$(BUILD)/%.o)
DEP := $(OBJ:.o=.d)
EXE := $(BUILD)/compiler

all: directories $(EXE)

directories:
	mkdir -p $(BUILD)

$(EXE): ${OBJ}
	-$(CXX) -o $@ $^ $(CXXFLAG)

$(BUILD)/%.o: %.cc
	-$(CXX) -c $< $(CXXFLAG) -MMD -MP -o $@

-include $(DEP)

.PHONY: directories all clean

clean:
	rm $(BUILD)/* 2>/dev/null || true;

