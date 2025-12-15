CC = cc
CFLAGS += -Wall -Wextra -g

TARGET = enseash
SRC = enseash.c tokenize.c redir.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

indent:
	indent -linux -i4 -nut -ts2 *.c

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean indent
