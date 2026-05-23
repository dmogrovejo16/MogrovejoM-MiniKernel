CC      = gcc
CFLAGS  = -Wall -Wextra -pthread -Iinclude
SRCS    = src/main.c src/queue.c src/cpu.c src/metrics.c
TARGET  = minikernel

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean ncpus
