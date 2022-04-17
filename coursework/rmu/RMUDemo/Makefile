SOURCES = $(TARGET).cpp convert.cpp
OBJECTS = $(SOURCES:.cpp=.o)
CXXFLAGS = -Wall -I../Include
LIBDIR = ../Libs
LIBS = -lrmu -liconv -lbz2 -lRMUResult
CXX = g++
TARGET = RMUDemo
all : $(TARGET) 

$(TARGET): $(OBJECTS) $(LIBDIR)/libRMUResult.a $(LIBDIR)/libbz2.a $(LIBDIR)/librmu.a
	$(CXX) -o $(TARGET) $(OBJECTS) -L$(LIBDIR) $(LIBS)
%.o : %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(LIBDIR)/libRMUResult.a:
	cd ../RMUResult; make -f Makefile
$(LIBDIR)/librmu.a:
	cd ../RMUDLL; make -f Makefile
$(LIBDIR)/libbz2.a:
	cd ../Bzip2/bzip2-1.0.6; make libbz2.a -f Makefile
	cp ../Bzip2/bzip2-1.0.6/libbz2.a $(LIBDIR)
-include deps.make
deps.make : $(SOURCES)
	$(CXX) -MM $(CXXFLAGS) $(SOURCES) > deps.make
clean:
	rm -f $(TARGET) $(OBJECTS) deps.make $(LIBDIR)/libbz2.a 
	cd ../RMUResult; make clean -f Makefile
	cd ../Bzip2/bzip2-1.0.6; make clean -f Makefile
	cd ../RMUDLL; make clean -f Makefile
