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

all: clean prog_server

prog_server: main_server.o server.o client.o
	$(CXX) main_server.o server.o client.o -o prog_server

main_server.o:
	$(CXX) $(CXXFLAGS) main_server.cpp

server.o:
	$(CXX) $(CXXFLAGS) src/server/server.cpp

client.o:
	$(CXX) $(CXXFLAGS) src/client/client.cpp

clean:
	rm -f main_server.o server.o client.o prog_server
