#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <cpu/memoria/disk> [argumentos]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "cpu") != 0 && strcmp(argv[1], "memory") != 0 && strcmp(argv[1], "disk") != 0) {
        fprintf(stderr, "El primer argumento debe ser 'cpu', 'memoria' o 'disk'\n");
        exit(EXIT_FAILURE);
    }

    int pipe_cpu[2], pipe_memoria[2], pipe_disk[2];
    
    if (pipe(pipe_cpu) == -1 || pipe(pipe_memoria) == -1 || pipe(pipe_disk) == -1) {
        perror("Error al crear los pipes");
        exit(EXIT_FAILURE);
    }

    pid_t pid_cpu = -1, pid_memoria = -1, pid_disk = -1;
    
    if (strcmp(argv[1], "cpu") == 0) {
        pid_cpu = fork();
        if (pid_cpu == 0) {
            close(pipe_cpu[0]);
            dup2(pipe_cpu[1], STDOUT_FILENO);
            execl("./cpu", "./cpu", (argc == 3) ? argv[2] : NULL, NULL);
            exit(EXIT_SUCCESS);
        }
    }
    else if (strcmp(argv[1], "memory") == 0) {
        pid_memoria = fork();
        if (pid_memoria == 0) {
            close(pipe_memoria[0]);
            dup2(pipe_memoria[1], STDOUT_FILENO); 
            execl("./memory", "./memory", argv[2], (argc == 4) ? argv[3] : NULL, NULL);
            exit(EXIT_SUCCESS);
        }
    }
    else if (strcmp(argv[1], "disk") == 0) {
        pid_disk = fork();
        if (pid_disk == 0) {
            close(pipe_disk[0]); 
            dup2(pipe_disk[1], STDOUT_FILENO); 
            execl("./disk", "./disk", (argc == 3) ? argv[2] : NULL, argv[argc - 1], NULL);
            exit(EXIT_SUCCESS);
        }
    }
    
    if (pid_cpu != -1) {
        waitpid(pid_cpu, NULL, 0);
    }
    if (pid_memoria != -1) {
        waitpid(pid_memoria, NULL, 0);
    }
    if (pid_disk != -1) {
        waitpid(pid_disk, NULL, 0);
    }
    
    close(pipe_cpu[1]);
    close(pipe_memoria[1]);
    close(pipe_disk[1]);
    
    char buffer[MAX_BUFFER_SIZE];
    
    if (strcmp(argv[1], "cpu") == 0) {
        printf("Datos del monitor de uso de CPU:\n");
        while (read(pipe_cpu[0], buffer, sizeof(buffer)) != 0) {
            printf("%s", buffer);
        }
    } else if (strcmp(argv[1], "memory") == 0) {
        printf("\nDatos del monitor de uso de memoria:\n");
        while (read(pipe_memoria[0], buffer, sizeof(buffer)) != 0) {
            printf("%s", buffer);
        }
    } else if (strcmp(argv[1], "disk") == 0) {
        printf("\nDatos del monitor de espacio en disco:\n");
        while (read(pipe_disk[0], buffer, sizeof(buffer)) != 0) {
            printf("%s", buffer);
        }
    }

    return 0;
}
