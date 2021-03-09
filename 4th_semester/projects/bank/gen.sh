#!/bin/sh

BASE_DIR=`pwd`
TOOLS_DIR=$BASE_DIR/src
tools=`ls $TOOLS_DIR`

#rm -rf Makefile
first_line="all: m.o"
for var in $tools; do
    first_line="$first_line $var.o"
done
echo $first_line
echo "	g++ -std=gnu++17 *.o -o run"
echo "m.o:"
echo "	g++ -std=gnu++17 -c main.cpp"

for var in $tools; do
	echo "$var.o:"
	echo "	g++ -std=gnu++17 -c src/$var/$var.cpp"
done

echo "clean:"
echo "	rm -rf *.o run"
