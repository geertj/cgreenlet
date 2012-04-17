INCLUDE = ../Makefile
include $(INCLUDE)
CFLAGS += -I.
CXXFLAGS += -I.

all: libgreenlet.a

libgreenlet.a: greenlet.o greenlet-cc.o greenlet-sys.o greenlet-asm.o
	$(AR) $(ARFLAGS) $@ $^

install:
	$(INSTALL) greenlet $(PREFIX)/include
	$(INSTALL) greenlet.h $(PREFIX)/include
	$(INSTALL) libgreenlet.a $(PREFIX)/lib

clean:
	$(RM) $(RMFLAGS) *.o libgreenlet.a
