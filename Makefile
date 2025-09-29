# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -g

# Project name and files
TARGET = pacsh
SRC = src/main.c

# Build rule
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean build artifacts
clean:
	rm -f $(TARGET)

