MAXMORPHSRC = ../SourceBase/MaxMorphFiles/RMStock.cpp ../SourceBase/MaxMorphFiles/RMStockData.cpp ../SourceBase/MaxMorphFiles/RMStockManager.cpp ../SourceBase/MaxMorphFiles/RMUnit.cpp 
MAXLIBSRC = ../SourceBase/MaxMorphFiles/Lib/FileConfig.cpp ../SourceBase/MaxMorphFiles/Lib/FileStream.cpp ../SourceBase/MaxMorphFiles/Lib/String.cpp ../SourceBase/MaxMorphFiles/Lib/Xception.cpp ../SourceBase/MaxMorphFiles/Lib/Xml.cpp 
SOURCEBASE = $(MAXMORPHSRC) $(MAXLIBSRC)
SOURCES = $(SOURCEBASE) Encode.cpp LexIterator.cpp RMUDLL.cpp
OBJECTS = $(SOURCES:.cpp=.o)
CXXFLAGS = -g -Wall -I../SourceBase/MaxMorphFiles -I../Include -DRMU_UTIL
LIBDIR = ../Libs
CXX = g++

all : librmu.a

librmu.a: $(OBJECTS)
	ar rcs $@ $^
	cp librmu.a $(LIBDIR)
%.o : %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@
-include deps.make
deps.make : $(SOURCES)
	$(CXX) -MM $(CXXFLAGS) $(SOURCES) > deps.make
test:
	echo $(OBJECTS)
clean:
	rm -f librmu.a $(LIBDIR)/librmu.a $(OBJECTS) deps.make
