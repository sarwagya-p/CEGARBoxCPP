#!/bin/bash 
g++ -std=c++17  -Wall -g $(find -name "*.cpp") -o main -lminisat -static -L./Prover/IpasirProver -lipasir
