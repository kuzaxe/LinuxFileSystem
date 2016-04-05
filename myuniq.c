#include <stdio.h>
#include <string.h>
#include <unistd.h>

int count = 0; /* 0=false, 1=true*/
int pass = 0;

extern void removeNewline (char *s);
extern void process(FILE *fp);

int main(int argc, char **argv) {
    int option;
    while ((option = getopt(argc, argv, ":c")) != -1) {
        switch (option) {
            case 'c':
                count=1;
                break;
            default:
                fprintf(stderr,"%s: invalid option -- '%c'\n", argv[0], (char)optopt);
                fprintf(stderr,"usage: %s [-c] [file ...]\n", argv[0]);
                return(1);
        }
    }

    FILE *fp;

    if (argc == 1 || argc == optind) {
        process(stdin);
    } else {
        for (; optind < argc ; optind++) {
            if (strcmp(argv[optind], "-") == 0) {
                process(stdin);
            } else if ((fp = fopen(argv[optind], "r")) == NULL) {
                perror(argv[optind]);
                pass = 1;
                continue;
            } else {
                process(fp);
                fclose(fp);
            }
        }
    }
    return(pass);
}

/* processes lines for uniqueness */
void process(FILE *fp) {

    char Line1[500];
    char Line2[500];
    int countRepeat = 0;

    int start=0;

    while(fgets(Line1, 500, fp) != NULL) {
        if (!(start == 0) && (strcmp(Line1, Line2) != 0)) {
            if (count  == 1)
                printf("%d ", countRepeat);
            removeNewline(Line2);
            printf("%s\n", Line2);
            countRepeat=1;
        } else
            countRepeat++;
        strcpy(Line2, Line1);
        start=1;
    }

    if (count == 1)
        printf("%d ", countRepeat);
    printf("%s", Line2);
}

/* Removes extra newline character -- caused double spaced results */
void removeNewline (char *s) {
   while(*s && *s != '\n' && *s != '\r')
       s++;
   *s = 0;
}
