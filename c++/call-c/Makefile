
TARGET = a.out

CC = g++
GCC = gcc

CFLAGS = -Wall
CXXFLAGS = -Wall

#LDFLAGS += -lpthread -lm

SRCS	+= $(wildcard *.cpp)
OBJS	= $(SRCS:.cpp=.o)

all: $(TARGET)

%.o: %.cpp
	$(CC) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -c -o $@ $^

$(TARGET): $(OBJS) hello.o
	$(CC) $(LDFLAGS) -o $@ $^

hello.o: hello.c
	$(GCC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f *.o $(TARGET)
