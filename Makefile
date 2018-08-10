TARGET = i2ctest

_OBJ = i2c_device.o \
	   i2c_test.o

_DEPS = i2c_device.h \
		log.h

IDIR = ./include
LDIR = ./lib
SDIR = ./
ODIR = .
LIBS = -lm

CC=gcc
CFLAGS=-I$(IDIR) -DLOG_LEVEL=3

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(TARGET)
