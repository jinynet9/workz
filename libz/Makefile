# Simple Makefile for znx library (static and shared)

# Compiler settings
CC      ?= gcc
CFLAGS  := -Wall -Wextra -fPIC -Iinclude
LDFLAGS := -shared

CFLAGS  += -I.

# Build directories
BUILD   := build_dir
LIBDIR  := lib

# Find all source files recursively
SRCS    := $(shell find . -name '*.c')
OBJS    := $(patsubst ./%.c,$(BUILD)/%.o,$(SRCS))
DEPS    := $(OBJS:.o=.d)

# Output libraries
STATIC_LIB := $(LIBDIR)/libznx.a
SHARED_LIB := $(LIBDIR)/libznx.so

# Default target - build shared library
all: $(SHARED_LIB)

# Static library target
static: $(STATIC_LIB)

# Create directories if they don't exist
$(BUILD) $(LIBDIR):
	@mkdir -p $@

# Build shared library
$(SHARED_LIB): $(OBJS) | $(LIBDIR)
	@echo "Linking shared library $@"
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

# Build static library
$(STATIC_LIB): $(OBJS) | $(LIBDIR)
	@echo "Creating static library $@"
	ar rcs $@ $(OBJS)

# Compile each source file
$(BUILD)/%.o: %.c | $(BUILD)
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include automatically generated dependencies
-include $(DEPS)

# Clean targets
clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf $(LIBDIR)

.PHONY: all static clean distclean
