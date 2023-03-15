#!/bin/bash 
g++ -std=c++17 -Wall -O3 -g $(find -name "*.cpp") -o main -lminisat -static
