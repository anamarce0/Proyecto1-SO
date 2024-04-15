CC = gcc
CFLAGS = -Wall -Wextra -g

PROGRAMS = main disk cpu memory

INSTALL_DIR = /usr/local/bin

all: $(PROGRAMS)

main: main.c
	$(CC) $(CFLAGS) -o main main.c

disk: disk.c
	$(CC) $(CFLAGS) -o disk disk.c

cpu: cpu.c
	$(CC) $(CFLAGS) -o cpu cpu.c

memory: memory.c
	$(CC) $(CFLAGS) -o memory memory.c

install: $(PROGRAMS)
	cp $(PROGRAMS) $(INSTALL_DIR)

clean:
	rm -f $(PROGRAMS)
