CC = gcc
RM = rm -rf

OBJS := sqlite3x.o

LIB_NAME = sqlite3x
CJSONX_STATIC = lib$(LIB_NAME).a
CJSONX_SHARED = lib$(LIB_NAME).so


CFLAGS = -g -Wall -Werror -fPIC -pthread -ldl -std=gnu99

.PHONY: clean static shared demo sqlite3

all: sqlite3 sqlite3_clean static shared

static: $(CJSONX_STATIC)

shared: $(CJSONX_SHARED)

demo: $(OBJS)
	$(CC) $^ demo.c libsqlite3.a -o $@ -I. $(CFLAGS) 

sqlite3:
	make -C sqlite3

sqlite3_clean:
	make clean -C sqlite3

$(CJSONX_STATIC): $(OBJS)
	@ $(AR) rcs $@ $^

$(CJSONX_SHARED): $(OBJS)
	@ $(CC) -shared -o $@ $(CFLAGS) $^

$(OBJS): %.o : %.c
	@ $(CC) $(CFLAGS)  -c $< -o $@

clean:
	@ $(RM) *.o $(CJSONX_STATIC) $(CJSONX_SHARED) demo
