#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc == 1){
        execlp("top", "top", "-n", "1", "-b", NULL);
    }else{
    char *pid = argv[1];
        execlp("top", "top", "-n", "1", "-b", "-p", pid, NULL);
    }
    return 0;
}
