TARGET = i2ctool

_OBJ = i2c_device.o \
	   i2c_test.o

_DEPS = i2c_api.h log.h

IDIR = ./
LDIR = ./lib
SDIR = ./
ODIR = .
LIBS = -lm

CC=gcc
CFLAGS=-I$(IDIR)

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(TARGET)
