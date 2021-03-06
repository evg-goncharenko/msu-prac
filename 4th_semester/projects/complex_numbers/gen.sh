#!/bin/sh

BASE_DIR=`pwd`
TOOLS_DIR=$BASE_DIR/tools
tools=`ls $TOOLS_DIR`

#rm -rf Makefile
first_line="all: m.o"
for var in $tools; do
    first_line="$first_line $var.o"
done
echo $first_line
echo "	g++ *.o -o run"
echo "m.o:"
echo "	g++ -c main.cpp"

for var in $tools; do
	echo "$var.o:"
	echo "	g++ -c tools/$var/$var.cpp"
done

echo "clean:"
echo "	rm -rf *.o run"
