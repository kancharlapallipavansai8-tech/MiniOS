CC = clang
CFLAGS = -Wall -Wextra -Iinclude -pthread
SRC = src/main.c src/shell.c src/monitor.c src/process.c
OBJ = $(SRC:.c=.o)
TARGET = minios

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
