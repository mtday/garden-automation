
CC = g++
CFLAGS = -g -Wall -std=c++11

default: all

%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

app: main.o
	$(CC) $(CFLAGS) -o espnow $^ -pthread


all: clean app

clean: 
	$(RM) -f espnow *.o

