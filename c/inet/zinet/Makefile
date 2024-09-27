
TARGET = udp_server udp_client

CC = gcc

CFLAGS = -Wall

INC = -I../../zlib

LDFLAGS += -lpthread

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $(EXTRA_CFLAGS) -c -o $@ $^

#$(TARGET): $(OBJS)
#	$(CC) $(LDFLAGS) -o $@ $^

udp_server: udp_server.o zinet.o
	$(CC) $(LDFLAGS) -o $@ $^

udp_client: udp_client.o zinet.o
	$(CC) $(LDFLAGS) -o $@ $^

sum_server: sum_server_stream.o zinet.o
	$(CC) $(LDFLAGS) -o $@ $^

sum_client: sum_client_stream.o zinet.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
