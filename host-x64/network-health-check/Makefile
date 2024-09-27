
TARGET = a.out

CC = gcc

CFLAGS = -Wall -DHOST_X64

LDFLAGS += -lpthread

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
