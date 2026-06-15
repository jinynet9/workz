
TARGET = a.out

CC = gcc

#CFLAGS = -Wall

LDFLAGS += -lpthread

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(TARGET)
#
#
# Symbol  Meaning                            Common Usage
# $@	  Target name	                     Compile output, link output
# $^	  All prerequisites (no duplicates)  Linking objects
# $<	  First prerequisite	             Compiling source files
# $?	  Prerequisites newer than target    Incremental builds
# $*	  Stem (pattern match)               Pattern rules (%.o : %.c)
# $(@D)	  Target directory                   Create output directories
# $(@F)	  Target file name                   Logging, debug
# $(<D)	  First prerequisite directory       Rare
# $(<F)	  First prerequisite file            Rare
#
