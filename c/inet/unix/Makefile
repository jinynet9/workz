
TARGET = udp_server udp_client sum_server sum_client

CC = gcc

CFLAGS = -Wall

LDFLAGS += -lpthread

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

#$(TARGET): $(OBJS)
#	$(CC) $(LDFLAGS) -o $@ $^

udp_server: udp_server.o
	$(CC) $(LDFLAGS) -o $@ $^

udp_client: udp_client.o
	$(CC) $(LDFLAGS) -o $@ $^

sum_server: sum_server_stream.o
	$(CC) $(LDFLAGS) -o $@ $^

sum_client: sum_client_stream.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
