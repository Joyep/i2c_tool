
CC=gcc
CFLAGS=-I$(IDIR)

IDIR =./include
LDIR =./lib
SDIR =./src
ODIR=obj

LIBS=-lm

_DEPS = i2c_api.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = i2c_api.o i2c_tool.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


i2ctool: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#$(ODIR)/%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

obj/i2c_api.o: src/i2c_api.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

obj/i2c_tool.o: src/i2c_tool.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
