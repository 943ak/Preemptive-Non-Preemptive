CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = Q1

all: $(TARGET)

$(TARGET): src/Q1.c
	$(CC) $(CFLAGS) src/Q1.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
