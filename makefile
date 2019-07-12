SRC = src/ECGroup.cpp src/ElGamalProtocol.cpp src/OrProtocol.cpp src/SchnorrProtocol.cpp src/SigmaProtocol.cpp src/Voter.cpp
TEST = test/ECGroupTest.cpp test/ElGamalProtocolTest.cpp test/Main.cpp test/OrProtocolTest.cpp test/SchnorrProtocolTest.cpp test/VoterTest.cpp
OBJ = $(SRC:src/%.cpp=build/%.o)
TESTOBJ = $(TEST:test/%.cpp=build/test/%.o)
DEP = $(OBJ:%.o=%.d)
TESTDEP = $(TESTOBJ:%.o=%.d)
FLAGS = -Wall -c -MMD -O3
LIBS = -lcryptopp
INC = -I include

runtests: $(OBJ) $(TESTOBJ)
	g++ $^ $(LIBS) -o $@

build/test/%.o: test/%.cpp
	g++ $< $(FLAGS) $(INC) $(LIBS) -o $@

build/%.o: src/%.cpp
	g++ $< $(FLAGS) $(INC) $(LIBS) -o $@

-include $(DEP) $(TESTDEP)

clean:
	rm -rf $(OBJ) $(TESTOBJ) $(DEP) $(TESTDEP)

.PHONY: clean
