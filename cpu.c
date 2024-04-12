#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 256

int main() {
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    double usuario = 0.0, sistema = 0.0;

    fp = popen("top -b -n 1| grep 'Cpu(s)'", "r");
    if (fp == NULL) {
        printf("Error al ejecutar el comando top\n");
        return -1;
    }

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%*s %lf %*s %lf", &usuario, &sistema);

    double porcentaje = usuario + sistema;
    printf("Porcentaje de uso del CPU: %.2f%%\n", porcentaje);

    return 0;
}
