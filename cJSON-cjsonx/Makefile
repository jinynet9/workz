
TARGET = test cjson

CC = gcc

CFLAGS += -Wall

LDFLAGS += -lpthread -lm

# -std=c89 -fPIC -pedantic -Wall -Wwrite-strings -Wshadow -Winit-self -Wcast-align -Wformat=2 -Wstrict-overflow=2 -Wcast-qual -Wc++-compat -Wundef -Wswitch-default -Wconversion -fstack-protector
# $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jinynet9/work/c/zlib

SRCS	+= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

#$(TARGET): $(OBJS)
#	$(CC) $(LDFLAGS) -o $@ $^

test: test.o cJSON.o
	$(CC) $(LDFLAGS) -o $@ $^

cjson: cjson.o cJSON.o zinet.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
