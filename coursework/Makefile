
CC = g++
CFLAGS = -O3 -Wall -std=c++11 -v

LIB2 = bz2
LIB1 = rmu
LIB_PATH = rmu/Libs

PROGRAM = analize.o

all : libanalize.a main


libanalize.a: analize.o 
	ar rcs $@ $^
	cp libanalize.a $(LIB_PATH)


analize.o: $(LIB)
	$(CC) $(CFLAGS) -I $(LIB_PATH) -L $(LIB_PATH) -c -o analize.o -DRMUDLL_EXPORTS  -DRMU_UTIL anz.cpp  -l $(LIB1) -l $(LIB2)

main:
	$(CC) $(CFLAGS) -I $(LIB_PATH) -L $(LIB_PATH) -o main main.cpp -l analize -l $(LIB1) -l $(LIB2)

clean:
	rm -f $(PROGRAM) main analize.o libanalize.a $(LIB_PATH)/libanalize.a
