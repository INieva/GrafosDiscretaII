TARGET=main
CC=gcc
CFLAGS=-std=gnu11 -Wall -Wextra -Wbad-function-cast -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -Werror -Werror=vla -g -pedantic -O3 -std=c99

# Propagar entorno a make en tests/
export CC  CFLAGS

SOURCES=$(shell echo *.c)
OBJECTS=$(SOURCES:.c=.o)

# Agregar objects-arch a los directorios de busqueda de los .o precompilados
ARCHDIR=objects-$(shell uname -m)
vpath $(ARCHDIR)
vpath $(ARCHDIR)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~
#	make -C tests clean

#test: $(OBJECTS)
#	make -C tests test

#memtest: $(OBJECTS)
#	make -C tests memtest

.depend: $(SOURCES)
	$(CC)  -MM $^ > $@

-include .depend

.PHONY: clean all
