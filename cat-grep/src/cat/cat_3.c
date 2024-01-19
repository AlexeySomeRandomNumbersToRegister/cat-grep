#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void processFile(FILE *file, int bflag, int eflag, int nflag, int sflag, int tflag);
void processSFlag(char *buffer, int *lastLineBlank, int sflag);
void processBFlag(char *buffer, int bflag, int *lineNumber);
void processNFlag(char *buffer, int nflag, int *lineNumber);
void processEFlag(char *buffer, int eflag);
void processTFlag(char *buffer, int tflag);

int main(int argc, char *argv[]) {
    int bflag = 0, eflag = 0, nflag = 0, sflag = 0, tflag = 0;
    int flags;

    while ((flags = getopt(argc, argv, "beEnstT")) != -1) {
        switch(flags) {
            case 'b':
                bflag++;
                break;
            case 'e':
            case 'E':
                eflag++;
                break;
            case 'n':
                nflag++;
                break;
            case 't':
            case 'T':
                tflag++;
                break;
            case 's':
                sflag++;
                break;
        }
    }

    int currentFileIndex = optind;

    while (currentFileIndex < argc) {
        FILE *file = fopen(argv[currentFileIndex], "r");
        if (file == NULL) {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[currentFileIndex]);
            exit(1);
        }

        processFile(file, bflag, eflag, nflag, sflag, tflag);
        fclose(file);

        currentFileIndex++;
    }

    return 0;
}

void processFile(FILE *file, int bflag, int eflag, int nflag, int sflag, int tflag) {
    char *buffer = NULL;
    size_t bufferSize = 0;
    int lastLineBlank = 0;
    int lineNumber = 1;

    while ((getline(&buffer, &bufferSize, file)) != -1) {
        processSFlag(buffer, &lastLineBlank, sflag);
        processBFlag(buffer, bflag, &lineNumber);
        processNFlag(buffer, nflag, &lineNumber);
        processEFlag(buffer, eflag);
        processTFlag(buffer, tflag);

        fprintf(stdout, "%s", buffer);
    }

    free(buffer);
}

void processSFlag(char *buffer, int *lastLineBlank, int sflag) {
    int length = strlen(buffer);
    if (sflag) {
        int currentLineBlank = (length <= 1) ? 1 : 0;
        if (*lastLineBlank && currentLineBlank) {
            buffer[0] = '\0';
        }
        *lastLineBlank = currentLineBlank;
    }
}

void processBFlag(char *buffer, int bflag, int *lineNumber) {
    int length = strlen(buffer);
    if (bflag && length > 1) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        sprintf(buffer, "%*d  ", 6, (*lineNumber)++);
        strcat(buffer, tmp);
        free(tmp);
    }
}

void processNFlag(char *buffer, int nflag, int *lineNumber) {
    if (nflag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        sprintf(buffer, "%*d  ", 6, (*lineNumber)++);
        strcat(buffer, tmp);
        free(tmp);
    }
}

void processEFlag(char *buffer, int eflag) {
    int length = strlen(buffer);
    if (eflag) {
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
            sprintf(buffer + length - 1, "%s", "$\n");
        } else {
            sprintf(buffer + length, "%s", "$");
        }
    }
}

void processTFlag(char *buffer, int tflag) {
    int length = strlen(buffer);
    if (tflag) {
        for (int i = 0; i < length; i++) {
            if (buffer[i] == '\t') {
                memmove(buffer + i + 2, buffer + i + 1, length - i);
                buffer[i] = '^';
                buffer[i + 1] = 'I';
                length++;
                i++;
            }
        }
    }
}

// Добавить: A
// Флаги -t с -n работают некорректно вместе: 36      37      38      39      40  Объяснения от самого линукса:
// Сделать обработку краевых ситуаций по типу неизвестного флага
// Придумать как в case запихнуть длинные названия флагов
// Устранить ошибку, что после использования неизвестного флага, файл все равно открывается
