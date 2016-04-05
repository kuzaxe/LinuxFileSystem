#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int printSize = 0, noRecur = 0, pass = 0;

extern int isDirectory (char *name);
extern int process (char *str);
extern void printing(char *fin, int size);

int main(int argc, char **argv) {
    int option;
    while ((option = getopt(argc, argv, ":sh")) != -1) {
        switch (option) {
            case 's':
                noRecur = 1;
                break;
            case 'h':
                printSize = 1;
                break;
            default:
                fprintf(stderr,"%s: invalid option -- '%c'\n", argv[0], (char)optopt);
                pass = 1;
                break;
        }
    }

    if (argc <= optind || pass == 1) {
        fprintf(stderr,"usage: %s [-sh] dir ...\n", argv[0]);
        return(1);
    }

    int size;
    for(; optind < argc; optind++) {
        size = 0;
        if (isDirectory(argv[optind])) {
            size = process(argv[optind]);
            if (noRecur)
                printing(argv[optind], size);
        } else {
            fprintf(stderr, "%s: Not a directory\n", argv[optind]);
        }
    }
    return(pass);
}

/* Returns true if 'name' argument is a directory. */
int isDirectory(char *name) {
    struct stat st;
    if (lstat(name, &st)) {
        perror(name);
        pass = 1;
    } else {
        if (S_ISDIR(st.st_mode))
            return(1);
    }
    return(0);
}

int process(char *str) {

    char path[2000] = "\0";
    strcat(path, str);
    int count = 0;

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(str)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;

            strcat(path, "/");
            if (strlen(ent->d_name) + strlen(path) > 2000) {
                fprintf(stderr, "The path/filename is too long.\n");
                pass = 1;
                continue;
            }

            strcat(path, ent->d_name);
            int size = 0;
            if (isDirectory(path))
                size = process(path);
            else {
                struct stat st;
                lstat(path, &st);
                count += st.st_blocks;
            }
            count += size;
            path[strlen(path) - strlen(ent->d_name) - 1] = '\0';
        }
        if (!noRecur)
            printing(path, count);
    } else {
        perror(str);
        pass = 1;
    }
    closedir(dir);
    return count;

}

/* Prints content based on options */
void printing(char *fin, int size) {
    int gig = 1024 * 1000;
    int meg = 1024;

    if (printSize) {
        if (size < meg) {
            printf("%dK %s\n", size/2, fin);
        } else if (meg <= size && size <= gig) {
            printf("%dM %s\n", (size+1024)/2048, fin);
        } else {
            printf("%dG %s\n", (size/1024)/(2048*1000), fin);
        }
    } else {
        printf("%d %s\n", size/2, fin);
    }
}
