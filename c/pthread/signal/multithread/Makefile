SHELL=/bin/sh

LIBOBJS=multithread.o

all : multithread

multithread : $(LIBOBJS)
	gcc -o $@ $? -lpthread 

clean:
	rm -f multithread *.o core *~

