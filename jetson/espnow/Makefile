
CC = gcc
CFLAGS = -g -Wall

default: all

%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

app: main.o
	$(CC) $(CFLAGS) -o espnow $^


all: clean app

clean: 
	$(RM) -f espnow *.o

