CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = scheduler

all: $(TARGET)

$(TARGET): src/scheduler.c
	$(CC) $(CFLAGS) src/scheduler.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
