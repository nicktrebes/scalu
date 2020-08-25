LUA = lua5.3
MKDIR = mkdir -p
RM = rm -rf

CFLAGS = -std=c99 -Iinclude -Wall -Werror -fPIC
LFLAGS = -shared

.PHONY: all bin clean test

LIB_SOURCES = $(wildcard src/*.c)
LIB_OBJECTS = $(addprefix bin/, $(notdir $(LIB_SOURCES:.c=.o)))
LIB_TARGET = bin/libscalu.so

all: $(LIB_TARGET)

bin:
	$(MKDIR) bin

clean:
	$(RM) bin

test: CFLAGS += -g
test: $(LIB_TARGET)
	$(foreach script, $(sort $(wildcard test/*.lua)), $(LUA) $(script))

$(LIB_TARGET): clean bin $(LIB_OBJECTS)
	$(CC) -o $(LIB_TARGET) $(LFLAGS) $(LIB_OBJECTS)

%.o:
	$(CC) -o $@ $(CFLAGS) -c src/$(basename $(notdir $@)).c
