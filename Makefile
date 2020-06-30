CC = g++
CFLAGS = -g -Wall
SRCS = alarm.cpp test.cpp
PROG = play

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) -lasound -lpthread

clean:
	rm *.o $(PROG)