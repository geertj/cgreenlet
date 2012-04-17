INCLUDE = ../Makefile
include $(INCLUDE)
CFLAGS += -I../src
CXXFLAGS += -I../src
LDFLAGS += -L../src

all: simple simple-cc

simple: simple.o
	$(CC) $(LDFLAGS) -o $@ $^ -lgreenlet

simple-cc: simple-cc.o
	$(CXX) $(LDFLAGS) -o $@ $^ -lgreenlet

clean:
	$(RM) $(RMFLAGS) simple simple.o simple-cc simple-cc.o
