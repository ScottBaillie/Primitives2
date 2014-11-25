
$(shell touch deps.mak)

sources := $(wildcard *.cc)
objs := $(sources:.cc=.o)


all: Test-server Test-client

Test-server: $(objs)
	gcc -pthread -o Test-server *.o -lstdc++ -lm

%.o : %.cc
	gcc -std=c++11 -c $< -o $@

deps:
	gcc -std=c++11 -M -c *.cc > deps.mak

clean:
	rm -f *.o
	rm -f Test-server
	rm -f Test-client

Test-client: $(objs)
	gcc -std=c++11 -DMSG_CLIENT -c Main.cc -o Main.o
	gcc -pthread -o Test-client *.o -lstdc++ -lm
	rm -f Main.o

include deps.mak
 
