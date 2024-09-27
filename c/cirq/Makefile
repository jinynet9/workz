
TARGET = a.out

CC = gcc

#CFLAGS = -Wall

# $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jinynet9/work/c/zlib
#LDFLAGS += -lpthread -lm -lzlib -L/home/jinynet9/work/c/zlib
LDFLAGS += -lpthread -lm


SRCS	+= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
