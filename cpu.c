#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    double usuario = 0.0, sistema = 0.0;
    char parametros[MAX_BUFFER_SIZE] = " " ;
    int pid1;


    if (argc == 1){
        fp = popen("top -b -n 1| grep 'Cpu(s)'", "r");
        if (fp == NULL) {
            sprintf(parametros, "Error al ejecutar el comando top\n");
            write(STDOUT_FILENO, parametros, strlen(parametros));
            return -1;
        }

        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "%*s %lf %*s %lf", &usuario, &sistema);

        double porcentaje = usuario + sistema;
        
        sprintf(parametros, "PORCENTAJE:%.2f%%\n", porcentaje);
        write(STDOUT_FILENO, parametros, strlen(parametros));
    }else if (argc == 2){
        int pid = atoi(argv[1]);
        char command[MAX_BUFFER_SIZE];
         snprintf(command, sizeof(command), "ps -p %d -o pid,pcpu,comm", pid);


        fp = popen(command, "r");
        if (fp == NULL) {
            sprintf(parametros, "Error al ejecutar el comando top\n");
            write(STDOUT_FILENO, parametros, strlen(parametros));
            return -1;
        }

        char buffer[MAX_BUFFER_SIZE];
        double porcentaje = 0.0;
        char nombre[25] = "";
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            pid1 = 0;
            porcentaje = 0.0;
           sscanf(buffer, "%d %lf %s", &pid1, &porcentaje, nombre);
           
        }
        sprintf(parametros, "PID:%d\nNOMBRE:%s\nPORCENTAJE DE UTILIZACION EN LOS ULTIMOS 5 MINUTOS:%.2lf%%\n", pid1, nombre, porcentaje);
        write(STDOUT_FILENO, parametros, strlen(parametros));
    }
    return 0;
}
