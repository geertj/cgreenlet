INCLUDE = ../Makefile
include $(INCLUDE)
CFLAGS += -I../src
CXXFLAGS += -I../src
LDFLAGS += -L../src

all: perf-setjmp perf-switch exception

perf-setjmp: perf-setjmp.o
	$(CC) $(LDFLAGS) -o $@ $^ -lgreenlet -lpthread

perf-switch: perf-switch.o
	$(CC) $(LDFLAGS) -o $@ $^ -lgreenlet -lpthread

exception: exception.o
	$(CXX) $(LDFLAGS) -o $@ $^ -lgreenlet -lpthread

clean:
	$(RM) $(RMFLAGS) perf-setjmp perf-switch exception *.o
