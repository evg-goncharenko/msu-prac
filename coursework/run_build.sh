#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NOCOLOR='\033[0m'
if cd rmu/Bzip2/bzip2-1.0.6 && make clean && make && cd ../../..;
then
    echo -e "${GREEN}Success build of bzip2${NOCOLOR}"
else
    echo -e "${RED}Build faild of bzip2${NOCOLOR}"
fi 
if cd rmu/RMUDLL && make clean && make && cd ../..
then
    echo -e "${GREEN}Success build of RMUDLL${NOCOLOR}"
else
    echo -e "${RED}Build faild of RMUDLL${NOCOLOR}"
fi
if make clean && make
then
    echo -e "${GREEN}Success build project${NOCOLOR}"
else
    echo -e "${RED}Build project faild${NOCOLOR}"
fi
