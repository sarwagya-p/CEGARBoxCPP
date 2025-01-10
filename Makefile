CC = g++
CFLAGS = -std=c++17 -Wall -g -ggdb -O3 -fopenmp #-mavx2
INCLUDE = -I./minisat_lib/include/ -I/opt/homebrew/Cellar/argp-standalone/1.5.0/include
LIBS = -L./minisat_lib/lib/ -lminisat  -Wl,-rpath,$(shell pwd)/minisat_lib/lib/ # -L/opt/homebrew/Cellar/argp-standalone/1.5.0/lib -largp #-ltcmalloc #-lprofiler #-Wl --no-as-needed

SRCDIR = .
SOURCES = $(shell find $(SRCDIR) -name "*.cpp")
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: minisat

minisat: minisat_lib
	cd minisat && \
	make config prefix=../minisat_lib/ && sudo make install

clean_minisat:
	rm -rf minisat_lib/*
