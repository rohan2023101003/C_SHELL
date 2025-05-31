CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99
SRCDIR = src
INCDIR = include
OBJDIR = obj
TARGET = myshell

# Get all .c files from src directory
SOURCES = $(wildcard $(SRCDIR)/*.c)
# Convert .c files to .o files in obj directory
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS) | $(OBJDIR)
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "Build successful! Executable: $(TARGET)"

# Compile .c files to .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean build artifacts
clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	@echo "Clean complete"

# Clean and rebuild
rebuild: clean all

# Install (optional - copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  rebuild  - Clean and build"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  uninstall- Remove from /usr/local/bin"
	@echo "  help     - Show this help"

# Declare phony targets
.PHONY: all clean rebuild install uninstall help