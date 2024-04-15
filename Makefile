CC = gcc
CFLAGS = -Wall -Wextra -g

PROGRAMS = main disk cpu memory

all: $(PROGRAMS)

main: main.c
	$(CC) $(CFLAGS) -o main main.c

disk: disk.c
	$(CC) $(CFLAGS) -o disk disk.c

cpu: cpu.c
	$(CC) $(CFLAGS) -o cpu cpu.c

memory: memory.c
	$(CC) $(CFLAGS) -o memory memory.c

clean:
	rm -f $(PROGRAMS)
