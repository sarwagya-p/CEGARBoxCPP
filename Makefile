CC = g++
CFLAGS = -std=c++17 -Wall -g -ggdb -O3 -mavx2 -fopenmp -static
LIBS = -L/usr/local/lib -lminisat #-ltcmalloc #-lprofiler #-Wl --no-as-needed

SRCDIR = .
SOURCES = $(shell find $(SRCDIR) -name "*.cpp")
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
