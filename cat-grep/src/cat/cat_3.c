#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void processFile(FILE *fp, int bflag, int eflag, int nflag, int sflag);
void printUsageAndExit(char *programName);

int main(int argc, char **argv) {
    int bflag = 0, eflag = 0, nflag = 0, sflag = 0;
    int opt;

    while ((opt = getopt(argc, argv, "bens?")) != -1) {
        switch(opt) {
            case 'b':
                bflag++;
                break;
            case 'e':
                eflag++;
                break;
            case 'n':
                nflag++;
                break;
            case 's':
                sflag++;
                break;
            case '?':
                printUsageAndExit(argv[0]);
        }
    }

    int currentFileIndex = optind;

    while (currentFileIndex < argc) {
        FILE *fp = fopen(argv[currentFileIndex], "rb");
        if (fp == NULL) {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[currentFileIndex]);
            exit(1);
        }

        processFile(fp, bflag, eflag, nflag, sflag);
        fclose(fp);

        currentFileIndex++;
    }

    return 0;
}

void processFile(FILE *fp, int bflag, int eflag, int nflag, int sflag) {
    const int bufferSize = BUFFER_SIZE;
    char buffer[bufferSize];
    int lastLineBlank = 0;
    int lineNumber = 1;

    while (fgets(buffer, bufferSize, (fp == NULL ? stdin : fp))) {
        int length = strlen(buffer);
        buffer[length - 1] = '\0';

        if (sflag) {
            int currentLineBlank = (length <= 1) ? 1 : 0;
            if (lastLineBlank && currentLineBlank) {
                continue;
            }
            lastLineBlank = currentLineBlank;
        }

        if (bflag) {
            if (length >= 1) {
                char *tmp = strdup(buffer);
                buffer[0] = '\0';
                sprintf(buffer, "%*d\t", 6, lineNumber++);
                strcat(buffer, tmp);
            }
        } else if (nflag) {
            char *tmp = strdup(buffer);
            buffer[0] = '\0';
            sprintf(buffer, "%*d\t", 6, lineNumber++);
            strcat(buffer, tmp);
        }

        if (eflag) {
            length = strlen(buffer);
            buffer[length] = '$';
            buffer[length + 1] = '\0';
        }

        fprintf(stdout, "%s\n", buffer);
    }
}

void printUsageAndExit(char *programName) {
    printf("usage: %s [-bens] [file ...]\n", programName);
    exit(1);
}