#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

char PATHS[2][1024];
extern int finder(char* comm);

int main(int argc, char **argv){

    /* Usage message */
    if (argc == 1) {
        fprintf(stderr, "usage: ./mywhich commandname ...\n");
        return(1);
    }

    /* PATH Locations */
    strcpy(PATHS[0], "/bin/");
    strcpy(PATHS[1], "/usr/bin/");
    strcpy(PATHS[2], "./");

    int i;
    for(i = 1; i < argc; i++) {
        finder(argv[i]);
    }
    return(0);
}

int finder(char* comm) {

    struct stat sb;

    int j;
    int numOfPath = 3;
    for (j = 0; j < numOfPath; j++) {
        if (strlen(PATHS[j] + strlen(comm)) > 1024) {
            fprintf(stderr, "Absolute path with argument too long.\n");
            return(1);
        }

        char path[10];
        strcpy(path, PATHS[j]);
        strcat(PATHS[j], comm);
        /* check if command is valid and executable */
        if ((stat(PATHS[j], &sb) >= 0) && (sb.st_mode > 0) && (S_IEXEC & sb.st_mode))  {
                printf("%s\n", PATHS[j]);
                strcpy(PATHS[j], path);
                return(0);
        }

        strcpy(PATHS[j], path);
    }

    fprintf(stderr, "%s: Command not found.\n", comm);
    return(1);
}
