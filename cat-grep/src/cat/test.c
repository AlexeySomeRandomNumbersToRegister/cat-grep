#include <stdio.h>
#include <string.h>

void printWordsWithDash(char *input) {
    char *token = strtok(input, " ");
    int isFirstWord = 1;  // Флаг, указывающий, что это первое слово

    while (token != NULL) {
        if (isFirstWord) {
            // Проверка, что первое слово соответствует "Привет"
            if (strcmp(token, "Привет") != 0) {
                printf("Первое слово должно быть 'Привет'.\n");
                return;
            }
            isFirstWord = 0;
        } else if (token[0] == '-' && strlen(token) > 1) {
            printf("%s ", token);
        } else if (token[0] != '-') {
            // Печать первого слова без "-", а затем прерывание цикла
            printf("%s\n", token);
            break;
        }
        
        token = strtok(NULL, " ");
    }
}

int main() {
    char input[100];
    printf("Введите слова: ");
    fgets(input, sizeof(input), stdin);

    // Убираем символ новой строки, который добавляется при использовании fgets
    input[strcspn(input, "\n")] = '\0';

    printWordsWithDash(input);

    return 0;
}
