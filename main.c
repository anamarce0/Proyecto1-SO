#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int pipe_cpu[2], pipe_memoria[2], pipe_disk[2];
    
    if (pipe(pipe_cpu) == -1 || pipe(pipe_memoria) == -1 || pipe(pipe_disk) == -1) {
        perror("Error al crear los pipes");
        exit(EXIT_FAILURE);
    }

    pid_t pid_cpu, pid_memoria, pid_disk;
    
    pid_cpu = fork();
    if (pid_cpu == 0) {
        close(pipe_cpu[0]);
        dup2(pipe_cpu[1], STDOUT_FILENO);
        execl("./cpu", "./cpu", (argc == 3) ? argv[2] : NULL, NULL);
        exit(EXIT_SUCCESS);
    }
    
    pid_memoria = fork();
    if (pid_memoria == 0) {
        close(pipe_memoria[0]);
        dup2(pipe_memoria[1], STDOUT_FILENO); 
        execl("./memory", "./memory", argv[2], (argc == 4) ? argv[3] : NULL, NULL);
        exit(EXIT_SUCCESS);
    }
    
    pid_disk = fork();
    if (pid_disk == 0) {
        close(pipe_disk[0]); 
        dup2(pipe_disk[1], STDOUT_FILENO); 
        execl("./disk", "./disk", (argc == 3) ? argv[2] : NULL, argv[argc - 1], NULL);
        exit(EXIT_SUCCESS);
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

    waitpid(pid_cpu, NULL, 0);
    waitpid(pid_memoria, NULL, 0);
    waitpid(pid_disk, NULL, 0);
    
    return 0;
}
