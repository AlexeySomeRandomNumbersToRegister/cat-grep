#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* parse_input(char* input);
char* read_file(char *filename);
void print_file(char* buffer);

int main() {
    char input[100];
    printf("Введите команду и флаги: ");
    fgets(input, sizeof(input), stdin);
    char* name_of_file = parse_input(input);
    
    if (name_of_file != NULL) {
        printf("Name of file: %s\n", name_of_file);
        char* file_content = read_file(name_of_file);
        print_file(file_content);
        free(file_content);
        free(name_of_file);
    }

    return 0;
}

char* parse_input(char* input) {
    char* token = strtok(input, " \t\n");
    
    if (token == NULL) {
        printf("Пустой ввод\n");
        return NULL;
    }

    if (strcmp("cat", token) != 0) {
        printf("Нет такой команды\n");
        exit(EXIT_FAILURE);
    }

    char* filename = NULL;
    char** flagList = NULL;
    int flagCount = 0;

    while ((token = strtok(NULL, " \t\n")) != NULL) {
        char* detected_flags = strstr(token, "-");
        if (detected_flags != NULL) {
            flagList = realloc(flagList, (flagCount + 1) * sizeof(char*));
            if (flagList == NULL) {
                printf("Ошибка выделения памяти\n");
                exit(EXIT_FAILURE);
            }

            flagList[flagCount] = strdup(token);
            if (flagList[flagCount] == NULL) {
                printf("Ошибка копирования флага\n");
                exit(EXIT_FAILURE);
            }

            printf("Обнаружен флаг: %s\n", token);
            flagCount++;
        } else {
            printf("Неизвестный аргумент: %s\n", token);
            filename = strdup(token);
            printf("TOKEN: %s\n", token);
        }
    }

    for (int i = 0; i < flagCount; i++) {
        printf("Обработка флага: %s\n", flagList[i]);
        free(flagList[i]);
    }
    free(flagList);

    return filename;
}

char* read_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(file_size + 1); // +1 для завершающего нуля
    if (buffer == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0'; // Завершаем строку нулём

    fclose(file);

    return buffer;
}

void print_file(char* buffer) {
    printf("%s", buffer);
}