
CFLAGS+= -g -Wall -lmystuff -L./

TARGET = x

all: $(TARGET)

%.o: %.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f x.o $(TARGET)
