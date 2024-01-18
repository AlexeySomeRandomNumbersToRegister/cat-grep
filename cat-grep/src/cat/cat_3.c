#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void processFile(FILE *file, int bflag, int eflag, int nflag, int sflag);

int main(int argc, char *argv[]) {
    int bflag = 0, eflag = 0, nflag = 0, sflag = 0;
    int flags;

    while ((flags = getopt(argc, argv, "bens")) != -1) {
        switch(flags) {
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
        }
    }

    int currentFileIndex = optind; // Индекс аргумента, который является файлом. Отрабатывает, когда флаги заканчиваются или их не было найдено

    while (currentFileIndex < argc) { // Этот цикл необходим для обработки нескольких файлов последовательно
        FILE *file = fopen(argv[currentFileIndex], "rb");
        if (file == NULL) {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[currentFileIndex]); // Выводится название исполняемого файла, 
            // название текущего файла, который не был найдет и текст ошибки.
            exit(1);
        }

        processFile(file, bflag, eflag, nflag, sflag);
        fclose(file);

        currentFileIndex++;
    }

    return 0;
}

void processFile(FILE *file, int bflag, int eflag, int nflag, int sflag) {
    const int bufferSize = BUFFER_SIZE; // Надо подумать как избавиться от этого
    char buffer[bufferSize];
    int lastLineBlank = 0;
    int lineNumber = 1;

    while (fgets(buffer, bufferSize, (file == NULL ? stdin : file))) { // Стандартный ввод нам необязательно обрабатывать, поэтому можно будет убрать
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


// Добавить: E, t, T, A, v, флаг b реализован некорректно
// Сделать обработку краевых ситуаций по типу неизвестного флага
// Придумать как в case запихнуть длинные названия флагов
// Устранить ошибку, что после использования неизвестного флага, файл все равно открывается
