SRC = src/Group.cpp src/ModularInt.cpp src/Schnorr.cpp src/SigmaUtils.cpp src/Elgamal.cpp
TEST = test/Main.cpp test/GroupTest.cpp test/ModularIntTest.cpp test/SchnorrTest.cpp test/SigmaUtilsTest.cpp test/ElgamalTest.cpp
OBJ = $(SRC:src/%.cpp=build/%.o)
TESTOBJ = $(TEST:test/%.cpp=build/test/%.o)
DEP = $(OBJ:%.o=%.d)
TESTDEP = $(TESTOBJ:%.o=%.d)
FLAGS = -Wall -g -c -MMD
INC = -I include -I lib

runtests: $(OBJ) $(TESTOBJ)
	g++ $^ -o $@

build/test/%.o: test/%.cpp
	g++ $(FLAGS) $(INC) $< -o $@

build/%.o: src/%.cpp
	g++ $(FLAGS) $(INC) $< -o $@

-include $(DEP) $(TESTDEP)

clean:
	rm -rf $(OBJ) $(TESTOBJ) $(DEP) $(TESTDEP)

.PHONY: clean
