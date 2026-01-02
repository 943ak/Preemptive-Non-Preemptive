CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = Q1

all: $(TARGET)

$(TARGET): Q1.c
	$(CC) $(CFLAGS) Q1.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
