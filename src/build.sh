#!/bin/bash

g++ -D_FILE_OFFSET_BITS=64 -g -lfuse test.cpp *block*.cpp filename.cpp archive.cpp *entry*.cpp fs.cpp
