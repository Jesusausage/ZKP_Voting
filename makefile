SRC = src/ECGroup.cpp src/SigmaProtocol.cpp src/Transcript.cpp src/ElGamalProtocol.cpp src/SchnorrProtocol.cpp # src/OrProtocol.cpp src/Voter.cpp src/KeyGen.cpp src/Verifier.cpp
TEST = test/ECGroupTest.cpp test/ElGamalProtocolTest.cpp test/Main.cpp test/SchnorrProtocolTest.cpp # test/OrProtocolTest.cpp test/VoterTest.cpp test/KeyGenTest.cpp test/VerifierTest.cpp
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
