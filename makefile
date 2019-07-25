SRC = src/ECGroup.cpp src/SigmaProtocol.cpp src/Transcript.cpp src/ElGamalProtocol.cpp src/SchnorrProtocol.cpp src/OrProtocol.cpp src/OrTranscript.cpp src/Voter.cpp src/Vote.cpp src/KeyGen.cpp src/Key.cpp src/Verifier.cpp src/Bootstrapper.cpp #src/Server.cpp
TEST = test/ECGroupTest.cpp test/ElGamalProtocolTest.cpp test/Main.cpp test/SchnorrProtocolTest.cpp test/OrProtocolTest.cpp test/VoterTest.cpp test/KeyGenTest.cpp test/VerifierTest.cpp test/BootstrapperTest.cpp
OBJ = $(SRC:src/%.cpp=build/%.o)
TESTOBJ = $(TEST:test/%.cpp=build/test/%.o)
DEP = $(OBJ:%.o=%.d)
TESTDEP = $(TESTOBJ:%.o=%.d)
FLAGS = -Wall -c -MMD -O3
LIBS = -lcryptopp -lpthread -lboost_system
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
