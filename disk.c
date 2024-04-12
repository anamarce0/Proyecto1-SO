#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <fcntl.h>  

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <-tm/-tg>\n", argv[0]);
        printf("-tm: Information in MiBs\n");
        printf("-tg: Information in GiBs\n");
        return 1;
    }

    char *unit = argv[1];
    if (strcmp(unit, "-tm") == 0) {
        unit = "-BM"; 
    } else if (strcmp(unit, "-tg") == 0) {
        unit = "-BG"; 
    } else {
        printf("Invalid option. Use -tm or -tg.\n");
        return 1;
    }

    execlp("df", "df", "-h", unit, "/mnt/c", NULL);

    return 0;
}