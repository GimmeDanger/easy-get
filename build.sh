#!/bin/sh

#compile
mkdir build
cd build
cmake ../
make
cp easy_get ../

#run some tests
./test_url
./test_parse_input
./test_http_queries

#cleanup after queries test
rm easy_get_filename_*
