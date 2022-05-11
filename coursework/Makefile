
CC = gcc
CFLAGS = -O3 -Wall -std=c++11 

LIB = rmu
LIB_PATH = rmu/Libs

PROGRAM = compound_analizer

all : $(PROGRAM)

$(PROGRAM): $(LIB)
	$(CC) $(CFLAGS) -I $(LIB_PATH) -L $(LIB_PATH) -o $(PROGRAM) main.cpp  -l $(LIB)

$(LIB):
	make rmu/RMUDLL
