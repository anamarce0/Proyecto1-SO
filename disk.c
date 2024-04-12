#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <fcntl.h>  

int main(int argc, char *argv[]) {

    char *unit = argv[1];
    if (strcmp(unit, "-tm") == 0) {
        unit = "-BM"; 
    } else if (strcmp(unit, "-tg") == 0) {
        unit = "-BG"; 
    } else {
        return 1;
    }

    execlp("df", "df", "-h", unit, "/mnt/c", NULL);

    return 0;
}
