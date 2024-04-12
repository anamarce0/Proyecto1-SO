#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    double usuario = 0.0, sistema = 0.0;
    double porcentaje = 0.0;

    if (argc == 1){
        fp = popen("top -b -n 1| grep 'Cpu(s)'", "r");
        if (fp == NULL) {
            printf("Error al ejecutar el comando top\n");
            return -1;
        }

        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "%*s %lf %*s %lf", &usuario, &sistema);

        porcentaje = usuario + sistema;
        printf("Porcentaje de uso del CPU: %.2f%%\n", porcentaje);
  
    }else if (argc == 2){
        int pid = atoi(argv[1]);
        char command[MAX_BUFFER_SIZE];
        snprintf(command, sizeof(command), "top -bn1 -p %d -d 300 | awk 'NR>7 && $1==%d {{print $9}}'", pid, pid);


        fp = popen(command, "r");
        if (fp == NULL) {
            printf("Error al ejecutar el comando top\n");
            return -1;
        }

        
        char buffer[MAX_BUFFER_SIZE];
        porcentaje = 0.0;
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            sscanf(buffer, "%lf", &porcentaje);
        }
        printf("Porcentaje de utilizacion del proceso %d en los ultinuto 5 : %.2f%%\n", pid, porcentaje);
    }
    return porcentaje;
}
