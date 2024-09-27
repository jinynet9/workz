
TARGET = bunny

CC = gcc

CFLAGS = -Wall -DDEBUG

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
