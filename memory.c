#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    double memoria_total = 0.0, memoria_usada = 0.0, porcentaje = 0.0, memVirtual, memReal;
    char parametros[MAX_BUFFER_SIZE] = " ";

    char *unit = argv[1];
    if (argc == 2) {
        if (strcmp(unit, "-r") == 0) {
            fp = popen("top -b -n 1| grep 'MiB Mem'", "r");
            if (fp == NULL) {
                sprintf(parametros, "Error al ejecutar el comando top\n");
                write(STDOUT_FILENO, parametros, strlen(parametros));
                return -1;
            }
            fgets(buffer, sizeof(buffer), fp);
            sscanf(buffer, "%*s %*s %*s %lf %*s %*s  %*s %lf", &memoria_total, &memoria_usada);

            porcentaje = (memoria_usada / memoria_total) * 100;

            sprintf(parametros, "PORCENTAJE:%.2f%%\n", porcentaje);
            write(STDOUT_FILENO, parametros, strlen(parametros));

        } else if (strcmp(unit, "-v") == 0) {
            fp = popen("top -b -n 1| grep 'MiB Swap'", "r");
            if (fp == NULL) {
                sprintf(parametros, "Error al ejecutar el comando top\n");
                write(STDOUT_FILENO, parametros, strlen(parametros));
                return -1;
            }
            fgets(buffer, sizeof(buffer), fp);
            sscanf(buffer, "%*s %*s %*s  %*s %lf %*s %lf", &memoria_total, &memoria_usada);

            porcentaje = (memoria_usada / memoria_total) * 100;

            sprintf(parametros, "PORCENTAJE:%.2f%%\n", porcentaje);
            write(STDOUT_FILENO, parametros, strlen(parametros));
        }
    } else if (argc == 3) {
        int pid = atoi(argv[2]);
        char command[MAX_BUFFER_SIZE];
        if (strcmp(unit, "-v") == 0) {
            snprintf(command, sizeof(command), "top -bn1 -p %d -c | awk 'NR>7 && $1==%d {print $12, $5/1024}'", pid, pid);

            fp = popen(command, "r");
            if (fp == NULL) {
                sprintf(parametros, "Error al ejecutar el comando top\n");
                write(STDOUT_FILENO, parametros, strlen(parametros));
                return -1;
            }

            char nombre[25] = "";
            if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                sscanf(buffer, "%s  %lf", nombre, &memVirtual);
            }

            fp = popen("free -m | awk 'NR==3{printf \"%.2f\\n\", ($2)}'", "r");
            if (fp == NULL) {
                sprintf(parametros, "Error al ejecutar el comando free\n");
                write(STDOUT_FILENO, parametros, strlen(parametros));
                return -1;
            }

            if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                sscanf(buffer, "%lf", &memoria_total);
            }

            porcentaje = ((memVirtual) / memoria_total) * 100;

            sprintf(parametros, "PID: %d\nNOMBRE: %s\nPORCENTAJE DE MEMORIA VIRTUAL: %.2f%%\n", pid, nombre, porcentaje);
            write(STDOUT_FILENO, parametros, strlen(parametros));
        } else {
            snprintf(command, sizeof(command), "top -bn1 -p %d | awk 'NR>7 && $1==%d {print $12, $10}'", pid, pid);

            fp = popen(command, "r");
            if (fp == NULL) {
                sprintf(parametros, "Error al ejecutar el comando top\n");
                write(STDOUT_FILENO, parametros, strlen(parametros));
                return -1;
            }

            char nombre[25] = "";
            if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                sscanf(buffer, "%s %lf", nombre, &memReal);
            }

            porcentaje = memReal;

            sprintf(parametros, "PID: %d\nNOMBRE:%s\nPORCENTAJE DE MEMORIA :%.2f%%\n", pid, nombre, porcentaje);
            write(STDOUT_FILENO, parametros, strlen(parametros));
        }
    }

    return 0;
}

