MAKE = make
CFLAGS = -g -O3 -Wall -pedantic -fexceptions 
CXXFLAGS = -g -O3 -Wall -pedantic
ARFLAGS = rv
ifeq ($(shell uname), Linux)
  CC = gcc
  CXX = g++ -std=c++0x
  RM = rm
  RMFLAGS = -f
  PREFIX = /usr/local
endif
ifeq ($(shell uname), Darwin)
  CC = clang
  CXX = clang++
  RM = rm
  RMFLAGS = -f
  PREFIX = /usr/local
endif
ifeq ($(shell uname), MINGW)
  CC = gcc
  CXX = g++
  RM = del
  RMFLAGS = 
  PREFIX = C:\greenlet
endif

ifeq ($(INCLUDE),)
all:
	$(MAKE) -C src $@
	$(MAKE) -C examples $@
	$(MAKE) -C test $@

clean:
	$(MAKE) -C src $@
	$(MAKE) -C examples $@
	$(MAKE) -C test $@
endif
