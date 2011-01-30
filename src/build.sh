#!/bin/bash

gcc -c unrarlib.c
g++ -D_FILE_OFFSET_BITS=64 -g -lfuse test.cpp fs.cpp archive.cpp *entry*.cpp *block*.cpp descriptor.cpp filename.cpp unrarlib.o
