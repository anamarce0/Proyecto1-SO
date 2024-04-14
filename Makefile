CC = gcc
CFLAGS = -Wall -Wextra -g

# Nombres de los programas
PROGRAMS = main memory disk cpu

# Directorio de instalación
INSTALL_DIR = /usr/local/bin

# Regla para compilar todos los programas
all: $(PROGRAMS)

# Reglas para compilar cada programa
main: main.c
  $(CC) $(CFLAGS) -o main main.c

memory: memory.c
	$(CC) $(CFLAGS) -o memory memory.c

disk: disk.c
	$(CC) $(CFLAGS) -o disk disk.c

cpu: cpu.c
	$(CC) $(CFLAGS) -o cpu cpu.c

# Regla para instalar los programas en /usr/local/bin
install: $(PROGRAMS)
	cp $(PROGRAMS) $(INSTALL_DIR)

# Regla para limpiar los archivos compilados
clean:
	rm -f $(PROGRAMS)
