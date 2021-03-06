all: m.o complex.o interface.o
	g++ *.o -o run
m.o:
	g++ -c main.cpp
complex.o:
	g++ -c tools/complex/complex.cpp
interface.o:
	g++ -c tools/interface/interface.cpp
clean:
	rm -rf *.o run
