CPPFLAGS+=-Wall -Wextra -Wpedantic
CPPFLAGS+=-Wwrite-strings -Wstack-usage=1024 -Wfloat-equal
CPPFLAGS+=-Waggregate-return -Winline

CFLAGS+=-std=c11

LDLIBS+=-lm

BIN=egg_drop
OBJS=egg_drop.o egg.o search.o

.PHONY: debug profile clean run val

all: $(BIN)

egg_drop: egg_drop.o egg.o search.o

debug: CFLAGS+=-g
debug: $(BIN)

profile: CFLAGS+=-pg
profile: LDFLAGS+=-pg
profile: $(BIN)

clean:
	$(RM) $(OBJS) $(BIN) 

run:
	./$(BIN)

val:
	valgrind --track-origins=yes ./$(BIN)
