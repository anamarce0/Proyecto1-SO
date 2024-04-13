#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    double memoria_total = 0.0, memoria_usada = 0.0, porcentaje = 0.0, memVirtual, memReal;
    char *unit = argv[1];
    if (argc == 2){
    if (strcmp(unit, "-r") == 0) {
            fp = popen("top -b -n 1| grep 'MiB Mem'", "r");
        if (fp == NULL) {
            printf("Error al ejecutar el comando top\n");
            return -1;
        }
        fgets(buffer, sizeof(buffer), fp);
       sscanf(buffer, "%*s %*s %*s %lf %*s %*s  %*s %lf", &memoria_total, &memoria_usada);

        porcentaje = (memoria_usada / memoria_total) * 100;

        // Imprimir lo que se ha extraído
        printf("Memoria Fisica total: %.2f MiB\n", memoria_total);
        printf("Memoria Fisica usada: %.2f MiB\n", memoria_usada);
        printf("Porcentaje de uso de memoria Fisica: %.2f%%\n", porcentaje);

    } else if (strcmp(unit, "-v") == 0) {
            fp = popen("top -b -n 1| grep 'MiB Swap'", "r");
        if (fp == NULL) {
            printf("Error al ejecutar el comando top\n");
            return -1;
        }
        fgets(buffer, sizeof(buffer), fp);
       sscanf(buffer, "%*s %*s %*s  %*s %lf %*s %lf", &memoria_total, &memoria_usada);

        porcentaje = (memoria_usada / memoria_total) * 100;

        printf("Memoria Virtual total: %.2f MiB\n", memoria_total);
        printf("Memoria  Virtual usada: %.2f MiB\n", memoria_usada);
        printf("Porcentaje de uso de memoria Virtual: %.2f%%\n", porcentaje);
        
    } 
    }else if (argc == 3){
            int pid = atoi(argv[2]);
            char command[MAX_BUFFER_SIZE];
            if (strcmp(unit, "-v") == 0) {   
                snprintf(command, sizeof(command), "top -bn1 -p %d | awk 'NR>7 && $1==%d {{print $5/1024}}'", pid, pid);
                
                fp = popen(command, "r");
                if (fp == NULL) {
                    printf("Error al ejecutar el comando top\n");
                    return -1;
                }
                if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                    sscanf(buffer, "%lf", &memVirtual);
                     printf("valor memoria virtual: %lf \n", memVirtual);
                }

                fp = popen("free -m | awk 'NR==3{printf \"%.2f\\n\", ($2)}'", "r");
                if (fp == NULL) {
                    printf("Error al ejecutar el comando free\n");
                    return -1;
                }
                

                if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                    sscanf(buffer, "%lf", &memoria_total);
                    printf("valor memoria: %lf \n", memoria_total);
                }

                porcentaje = ((memVirtual)/memoria_total) *100;
                printf("Porcentaje de utilizacion del proceso %d en los ultinuto 5 : %.2f%%\n", pid, porcentaje);
            }else {
                snprintf(command, sizeof(command), "top -bn1 -p %d | awk 'NR>7 && $1==%d {{print $10}}'", pid, pid);
                
                fp = popen(command, "r");
                if (fp == NULL) {
                    printf("Error al ejecutar el comando top\n");
                    return -1;
                }
                if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                    sscanf(buffer, "%lf", &memReal);
                     printf("valor memoria virtual: %lf \n", memReal);
                }

                 porcentaje = memReal;
                printf("Porcentaje de utilizacion del proceso %d en los ultinuto 5 : %.2f%%\n", pid, porcentaje);

            }

            
        }

       
    return 0;
}
