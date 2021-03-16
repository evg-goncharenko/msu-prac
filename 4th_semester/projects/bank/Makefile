BUILD ?= debug
#BUILD = release

CXX = g++ -std=gnu++17
CXXFLAGS = -c

ifeq ($(BUILD), debug)
CXXFLAGS += -Wall -O0
endif

ifeq ($(BUILD), release)
CXXFLAGS += -O3
endif

all: clean prog

prog: main.o bank.o interface.o
	$(CXX) main.o bank.o interface.o -o prog

main.o:
	$(CXX) $(CXXFLAGS) main.cpp

bank.o:
	$(CXX) $(CXXFLAGS) src/bank/bank.cpp

interface.o:
	$(CXX) $(CXXFLAGS) src/interface/interface.cpp

clean:
	rm -f main.o bank.o interface.o prog
