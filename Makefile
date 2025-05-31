CC = gcc
CFLAGS = -D_GNU_SOURCE -D_DEFAULT_SOURCE
SRCDIR = src
TARGET = my_shell

# List all source files
SOURCES = $(SRCDIR)/log.c $(SRCDIR)/reveal.c $(SRCDIR)/main.c $(SRCDIR)/hop.c \
          $(SRCDIR)/foreground.c $(SRCDIR)/background.c $(SRCDIR)/proclore.c \
          $(SRCDIR)/seek.c $(SRCDIR)/display.c $(SRCDIR)/myshrc.c $(SRCDIR)/IO.c \
          $(SRCDIR)/pipe.c $(SRCDIR)/activities.c $(SRCDIR)/sig.c $(SRCDIR)/custom.c \
          $(SRCDIR)/fg.c $(SRCDIR)/process.c $(SRCDIR)/bg.c $(SRCDIR)/iMan.c $(SRCDIR)/neonate.c

# Default target - compile all at once like your original
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -Iinclude $(SOURCES) -o $(TARGET)
	@echo "Build successful! Executable: $(TARGET)"

# Clean build artifacts
clean:
	rm -f $(TARGET)
	@echo "Clean complete"

# Clean and rebuild
rebuild: clean all

# Declare phony targets
.PHONY: all clean rebuild