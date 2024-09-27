
TARGET = echos echoc

CC = gcc

#CFLAGS = -Wall

LDFLAGS += -lpthread

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

#$(TARGET): $(OBJS)
#	$(CC) $(LDFLAGS) -o $@ $^

echos: echo_server.o
	$(CC) $(LDFLAGS) -o $@ $^

echoc: echo_client.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
