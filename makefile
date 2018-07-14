CC = g++
CFLAGS = -std=c++11 -I$(IDIR) -g

IDIR = ./include/
SRCDIR = ./src/

SOURCES = $(SRCDIR)*.cpp\

all: circuit

circuit: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o $@

run:
	./circuit

clean:
	rm circuit