CC = gcc
RM = rm -rf
MV = mv

# Get C sources
OBJECTS = shell.o sqlite3.o

LIB_NAME ?= sqlite3
STATIC_NAME ?= lib$(LIB_NAME).a

CXXFLAGS = -g -Wall -fPIC -lm -lrt -I. -pthread -ldl

all: static_lib
	@ $(MV) $(STATIC_NAME) ../$(STATIC_NAME)
	
static_lib: $(OBJECTS)
	@ $(AR) -crs $(STATIC_NAME) *.o 

$(OBJECTS): %.o : %.c
	@ $(CC) -c $< -o $@ $(CXXFLAGS) -O0

.PHONY: clean
clean:
	@ $(RM) *.o *.a *.so
