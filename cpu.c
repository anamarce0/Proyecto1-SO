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
    double porcentaje = 0.0;


    if (argc == 1){
        fp = popen("top -b -n 1| grep 'Cpu(s)'", "r");
        if (fp == NULL) {
            sprintf(parametros, "Error al ejecutar el comando top\n");
            write(STDOUT_FILENO, parametros, strlen(parametros));
            return -1;
        }

        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "%*s %lf %*s %lf", &usuario, &sistema);

        porcentaje = usuario + sistema;
        
        sprintf(parametros, "PORCENTAJE:%.2f%%\n", porcentaje);
        write(STDOUT_FILENO, parametros, strlen(parametros));
    }else if (argc == 2){
        int pid = atoi(argv[1]);
        
        snprintf(buffer, sizeof(buffer), "pidstat -u -p %d 60 5 > cpu_usage.log &", pid);
        system(buffer);

        sleep(300);

        fp = popen("awk '$1 == \"Average:\" {print $8}' cpu_usage.log", "r");
        if (fp == NULL) {
            sprintf(parametros, "Error al ejecutar el comando awk\n");
            write(STDOUT_FILENO, parametros, strlen(parametros));
            return -1;
        }

        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            porcentaje = atof(buffer); 
        }

    
        snprintf(buffer, sizeof(buffer), "/proc/%d/comm", pid);
        fp = fopen(buffer, "r");
        if (fp == NULL) {
            sprintf(parametros,"Error al abrir el archivo /proc/%d/comm\n", pid);
            write(STDOUT_FILENO, parametros, strlen(parametros));
            return -1;
        }
        char nombre[25] = "";
        if (fgets(nombre, sizeof(nombre), fp) != NULL) {
            nombre[strcspn(nombre, "\n")] = '\0';
        }

        fclose(fp);

        snprintf(parametros, sizeof(parametros), "PID:%d\nNOMBRE:%s\nPORCENTAJE DE UTILIZACION EN LOS ULTIMOS 5 MINUTOS:%.2lf%%\n", pid, nombre, porcentaje);
        write(STDOUT_FILENO, parametros, strlen(parametros));
    }
    return 0;
}
