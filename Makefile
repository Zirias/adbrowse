ifdef DEBUG
CFLAGS ?= -O0 -g3
else
CFLAGS ?= -O3 -g0
endif

CC ?= gcc

CFLAGS += -Wall -Wextra -pedantic

ADBHOST_OBJS = adbhost.o adbconnect_win32.o stringbuilder.o bufreader.o

all: adbhost.exe

%.o: %.c
	$(CC) -c $(CFLAGS) -o$@ $^

adbhost.exe: $(ADBHOST_OBJS)
	$(CC) -o$@ $^

clean:
	rm -f *.o *.exe

.PHONY: all clean
