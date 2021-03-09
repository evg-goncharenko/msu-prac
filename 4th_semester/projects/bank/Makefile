all: m.o bank.o interface.o
	g++ -std=gnu++17 *.o -o run
m.o:
	g++ -std=gnu++17 -c main.cpp
bank.o:
	g++ -std=gnu++17 -c src/bank/bank.cpp
interface.o:
	g++ -std=gnu++17 -c src/interface/interface.cpp
clean:
	rm -rf *.o run
