#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s cpu PID\n", argv[0]);
        return 1;
    }

    // Obtener el PID del argumento
    int pid = atoi(argv[2]);

    // Construir el comando para obtener la información del proceso con el PID especificado durante un minuto
    char command[MAX_BUFFER_SIZE];
    snprintf(command, sizeof(command), "top -bn1 -p %d | awk 'NR>7 && $1==%d {{print $9}}'", pid, pid);

    // Ejecutar el comando y leer la salida
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        printf("Error al ejecutar el comando top\n");
        return -1;
    }

    // Leer la salida del comando
    char buffer[MAX_BUFFER_SIZE];
    double porcentaje = 0.0;
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        sscanf(buffer, "%lf", &porcentaje);
    }

    // Imprimir el porcentaje de utilización del proceso
    printf("Porcentaje de utilizacion del proceso %d en el ultimo minuto: %.2f%%\n", pid, porcentaje);

    return 0;
}
