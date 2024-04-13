#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    double usuario = 0.0, sistema = 0.0;
    char parametros[100];
    char nombre[10];
    int pid1;


    if (argc == 1){
        fp = popen("top -b -n 1| grep 'Cpu(s)'", "r");
        if (fp == NULL) {
            printf("Error al ejecutar el comando top\n");
            return -1;
        }

        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "%*s %lf %*s %lf", &usuario, &sistema);

        double porcentaje = usuario + sistema;
        
       
        sprintf(parametros, "PORCENTAJE:%.4f%%%%", porcentaje);
    }else if (argc == 2){
        int pid = atoi(argv[1]);
        char command[MAX_BUFFER_SIZE];
         snprintf(command, sizeof(command), "top -bn1 -p %d -d 300 | awk 'NR>7 && $1==%d'", pid, pid);


        fp = popen(command, "r");
        if (fp == NULL) {
            printf("Error al ejecutar el comando top\n");
            return -1;
        }

        char buffer[MAX_BUFFER_SIZE];
        double porcentaje = 0.0;
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
           sscanf(buffer, "%d %*s %*s %*s %*s %*s %*s %*s %*s %lf %*s %s", &pid1, &porcentaje, nombre);
           
        }
        sprintf(parametros, "PID:%d, NOMBRE:%s, PORCENTAJE:%.2lf", pid1, nombre, porcentaje);

    }
    return 0;
}
