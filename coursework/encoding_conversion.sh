#!/bin/bash
find ./ -name "*.cpp" -o -name "*.c" -o -name "*.hpp" -o -name "*.h"  -type f |
while read file
do
  echo " $file"
  mv $file $file.icv
  iconv -f WINDOWS-1251 -t UTF-8 $file.icv > $file
  rm -f $file.icv
done