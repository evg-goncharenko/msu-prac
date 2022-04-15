SOURCES = TestRMU.cpp convert.cpp
OBJECTS = $(SOURCES:.cpp=.o)
CXXFLAGS = -g -Wall -I../Include -std=c++11
LIBDIR = ../Libs
CXX = g++
LIBS = -lrmu -liconv -lbz2

all : TestRMU

TestRMU: $(OBJECTS)
	$(CXX) -o TestRMU $(OBJECTS) $(LIBS) -L$(LIBDIR)
%.o : %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@
-include deps.make
deps.make : $(SOURCES)
	$(CXX) -MM $(CXXFLAGS) $(SOURCES) > deps.make
test:
	echo $(OBJECTS)
clean:
	rm -f TestRMU $(OBJECTS) deps.make
