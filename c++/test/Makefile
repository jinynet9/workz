
TARGET = a.out

CC = g++

#CXXFLAGS = -Wall

LDFLAGS += -lpthread -lm

SRCS	+= $(wildcard *.cpp)
OBJS	= $(SRCS:.cpp=.o)

all: $(TARGET)

%.o: %.cpp
	$(CC) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -c -o $@ $^

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
