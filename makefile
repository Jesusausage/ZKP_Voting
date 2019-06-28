SRC = src/Group.cpp src/ModularInt.cpp src/Schnorr.cpp
OBJ = $(SRC:src/%.cpp=build/%.o)
DEP = $(OBJ:%.o=%.d)
FLAGS = -Wall -g -c -MMD
INC = -I include -I lib

all: bin/ModularIntTest bin/GroupTest

bin/ModularIntTest: $(OBJ) build/ModularIntTest.o
	g++ $^ -o $@

bin/GroupTest: $(OBJ) build/GroupTest.o
	g++ $^ -o $@

build/%.o: src/%.cpp
	g++ $(FLAGS) $(INC) $< -o $@

build/%.o: test/%.cpp
	g++ $(FLAGS) $(INC) $< -o $@

-include $(DEP)

clean:
	rm -rf build/* bin/*

.PHONY: clean
