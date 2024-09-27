
TARGET = libzlib.so

CC = gcc

CFLAGS = -Wall -fPIC

#LDFLAGS += -lpthread

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $^

clean:
	rm -f *.o $(TARGET)
