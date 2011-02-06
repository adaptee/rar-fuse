#!/bin/bash

#gcc -c unrarlib.c

g++ -g -D_FILE_OFFSET_BITS=64\
    -I  ../../unrar \
    -L  ../../unrar \
    -lfuse \
    -ldecompress \
    test.cpp fs.cpp archive.cpp *entry*.cpp *block*.cpp descriptor.cpp filename.cpp dostime.cpp

    #../../unrar/libdecompress.so \
