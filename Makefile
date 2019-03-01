CC = gcc
CFLAGS = -Wall -g -std=c99
OBJECT1 = smallsh.o
PROG1 = smallsh

all: $(PROG1)
	./$(PROG1)

$(PROG1): $(OBJECT1)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c  $(CFLAGS) $^

clean:
	rm -rf $(OBJECT1) $(PROG1)
