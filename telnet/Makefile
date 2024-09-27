
TARGET = telnetd telnet

CC = gcc

#CFLAGS = -Wall

LDFLAGS += -lpthread

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

telnetd: telnetd.o
	$(CC) $(LDFLAGS) -o $@ $^

telnet: telnet.o
	$(CC) $(LDFLAGS) -o $@ $^

#$(TARGET): $(OBJS)
#	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
