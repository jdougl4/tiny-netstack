# Compiler
CC = gcc

# Compiler flags
#
# -Wall -> enable common warnings
# -Wextra -> enable extra warnings
# -g -> include debug symbols
# -O0 -> disabling optimization for easier debugging
# -Iinclude -> tell the compiler where header files are
CFLAGS = -Wall -Wextra -g -O0 -Iinclude

# Final executable
TARGET = tiny.netstack

# Source files
SRC = \
      src/main.c \
      src/tap.c \
      src/ethernet.c \
      src/util.c

# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Linking final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Compile source files into objects
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean
