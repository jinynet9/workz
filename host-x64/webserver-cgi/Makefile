
TARGET = webcli

CC = gcc

CFLAGS = -Wall -I./lib -fstack-protector -DHOST_X64
CXXFLAGS = -Wall -I./lib -fstack-protector -DHOST_X64

#LDFLAGS += -lpthread -lcrypt
LDFLAGS += -lcrypt

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

OBJS =	\
	webcli.o \
	title.o \
	sysinfo.o \
	lan_config.o \
	system_setup.o \
	firewall.o \
	remote_access.o \
	syslog.o \
	time_setup.o \
	wan_config.o \
	upgrade.o \
	engineer_mode.o \
	lib/session.o \
	lib/cgi-lib.o \
	lib/cgi-util.o \
	lib/zut.o \
	lib/zinet.o \
	lib/wcx.o \
	lib/crypt.o \
	lib/pf_api.o \
	lib/qc_api.o \
#

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^

#$(TARGET): $(OBJS)
#	$(CC) $(LDFLAGS) -o $@ $^

webcli: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
