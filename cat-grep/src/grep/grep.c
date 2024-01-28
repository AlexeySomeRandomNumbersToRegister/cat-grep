// ВСЁ КРОМЕ -е ОТЛИЧНО РАБОТАЕТ !! !! !! !! !! !! !! !! !! !! !! !! 

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <getopt.h>

#define ANSI_COLOR_RED "\033[1;31m"
#define ANSI_COLOR_RESET "\x1b[0m"

void parse_args(int argc, char *argv[], int *e_flag, int *i_flag, int *v_flag, int *c_flag, int *l_flag, int *n_flag, int *h_flag, int *s_flag, int *f_flag, int *o_flag);
FILE *open_file(const char *filename, char *argv[]);
void finder(FILE *file, const char *filename, char *required_data, int v_flag, int i_flag, int c_flag, int n_flag, int e_flag, int l_flag);
void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number, int n_flag, int *smth_found);
void process_i_flag(char *buffer, char *required_data, int i_flag, int *line_number, int n_flag, int v_flag, int *smth_found);
void process_n_flag(char *buffer, char *required_data, int n_flag, int *line_number, int i_flag, int c_flag, int v_flag, int *smth_found);

int main(int argc, char *argv[]){
    int e_flag = 0, i_flag = 0, v_flag = 0, c_flag = 0, l_flag = 0, n_flag = 0, h_flag = 0, s_flag = 0, f_flag = 0, o_flag = 0; 
    parse_args(argc, argv, &e_flag, &i_flag, &v_flag, &c_flag, &l_flag, &n_flag, &h_flag, &s_flag, &f_flag, &o_flag);
    char *required_data = argv[optind];
    for (int i = optind + 1; i < argc; i++){
        FILE *file = open_file(argv[i], argv);
        finder(file, argv[i], required_data, v_flag, i_flag, c_flag, n_flag, e_flag, l_flag);
        fclose(file);
    }
    return 0;
}

void parse_args(int argc, char *argv[], int *e_flag, int *i_flag, int *v_flag, int *c_flag, int *l_flag, int *n_flag, int *h_flag, int *s_flag, int *f_flag, int *o_flag){
    int flags;
    while((flags = getopt(argc, argv, "e:ivclnhsfo")) != -1){
        switch(flags){
            case 'e':
                (*e_flag)++;
                required_data = optarg;
                break;
            case 'i':
                (*i_flag)++;
                break;
            case 'v':
                (*v_flag)++;
                break;
            case 'c':
                (*c_flag)++;
                break;
            case 'l':
                (*l_flag)++;
                break;
            case 'n':
                (*n_flag)++;
                break;
            case 'h':
                (*h_flag)++;
                break;
            case 's':
                (*s_flag)++;
                break;
            case 'f':
                (*f_flag)++;
                break;
            case 'o':
                (*o_flag)++;
                break;
            default:
                fprintf(stderr, "Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (argc == optind) {
        fprintf(stderr, "Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

FILE *open_file(const char *filename, char *argv[]){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

// Работает, но не выделяет красным
void finder(FILE *file, const char *filename, char *required_data, int v_flag, int i_flag, int c_flag, int n_flag, int e_flag, int l_flag){
    char *buffer = NULL;
    size_t buffer_size = 0;
    int line_number = 0;  // Инициализируем счетчик строк
    int smth_found = 0;

    while((getline(&buffer, &buffer_size, file)) != -1){
        process_v_flag(buffer, required_data, v_flag, i_flag, &line_number, n_flag, &smth_found);
        process_i_flag(buffer, required_data, i_flag, &line_number, n_flag, v_flag, &smth_found);
        process_n_flag(buffer, required_data, n_flag, &line_number, i_flag, c_flag, v_flag, &smth_found);

        if(!v_flag && !i_flag && !n_flag) {

            char *tmp = strdup(buffer);
            buffer[0] = '\0';
            if(strstr(tmp, required_data) != NULL){
                strcat(buffer, tmp);
                line_number++;  // Увеличиваем счетчик только при обнаружении совпадения
                smth_found = 1;
            }
            free(tmp);
        }
        if(!c_flag && !l_flag) {
            fprintf(stdout, "%s", buffer);
        }
    }
        if (c_flag && !l_flag) {
            fprintf(stdout, "%d\n", line_number);
        }
        if (l_flag && smth_found){
            printf("%s\n", filename);
        }
    free(buffer);
}

void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number, int n_flag, int *smth_found){
    if(v_flag && !n_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!i_flag && strstr(tmp, required_data) == NULL) {
            strcat(buffer, tmp);
            (*line_number)++;
            (*smth_found) = 1;
        }
        if (i_flag && strcasestr(tmp, required_data) == NULL) {
            strcat(buffer, tmp);
            (*line_number)++;
            (*smth_found) = 1;
        }
        free(tmp);
    }
}

void process_i_flag(char *buffer, char *required_data, int i_flag, int *line_number, int n_flag, int v_flag, int *smth_found){
    if(i_flag && !v_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if(strcasestr(tmp, required_data) != NULL) {
            strcat(buffer, tmp);
            (*smth_found) = 1;
            if (!n_flag) {
                (*line_number)++;
            } 
        }
        free(tmp);
    }
}

void process_n_flag(char *buffer, char *required_data, int n_flag, int *line_number, int i_flag, int c_flag, int v_flag, int *smth_found) {
    if (n_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!c_flag) {
            (*line_number)++;
        }
        if (!i_flag && !v_flag) {
            if(strstr(tmp, required_data) != NULL) {
                sprintf(buffer, "%d:", *line_number);
                strcat(buffer, tmp);
                (*smth_found) = 1;
                if(c_flag) {
                    (*line_number)++;
                }
            }
        }
        if (i_flag && !v_flag) {
            if(strcasestr(tmp, required_data) != NULL) {
                sprintf(buffer, "%d:", *line_number);
                strcat(buffer, tmp);
                (*smth_found) = 1;
                if(c_flag) {
                    (*line_number)++;
                }
            }
        }
        if (v_flag && !i_flag) {
            if(strstr(tmp, required_data) == NULL) {
                sprintf(buffer, "%d:", *line_number);
                strcat(buffer, tmp);
                (*smth_found) = 1;
                if(c_flag) {
                    (*line_number)++;
                }
            }
        }
        if (v_flag && i_flag) {
            if(strcasestr(tmp, required_data) == NULL) {
                sprintf(buffer, "%d:", *line_number);
                strcat(buffer, tmp);
                (*smth_found) = 1;
                if(c_flag) {
                    (*line_number)++;
                }
            }
        }
    free(tmp);
    }
}

// // РАБОТАЕТ, НАДО РАЗОБРАТЬСЯ
// void finder(FILE *file, char *required_data) {
//     char *buffer = NULL;
//     size_t buffer_size = 0;
    
//     while ((getline(&buffer, &buffer_size, file) != -1)) {
//         char *match = buffer;
//         char *next_match;
//         int match_found = 0;

//         while ((next_match = strstr(match, required_data)) != NULL) {
//             size_t prefix_len = next_match - match;

//             // Вывод префикса
//             if (prefix_len > 0) {
//                 fwrite(match, 1, prefix_len, stdout);
//             }

//             // Выделение найденного текста красным цветом
//             fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, required_data, ANSI_COLOR_RESET);

//             // Перемещение указателя на следующий символ после найденной подстроки
//             match = next_match + strlen(required_data);

//             match_found = 1;
//         }

//         // Вывод оставшейся части строки, если не найдено больше совпадений
//         if (match_found && *match != '\0') {
//             fputs(match, stdout);
//         }
//     }
// }

// СТАРАЯ ВЕРСИЯ ФУНКЦИИ
// void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number){
//     if (v_flag) {
//         char *tmp = strdup(buffer);
//         buffer[0] = '\0';
//         if (!i_flag) {
//             if (strstr(tmp, required_data) == NULL) {
//                 (*line_number)++;
//             } else {
//                 strcat(buffer, tmp);
//             }
//         }
//         if (i_flag) {
//             if (strcasestr(tmp, required_data) == NULL) {
//                 (*line_number)++;
//             } else {
//                 strcat(buffer, tmp);
//             }
//         }
//         free(tmp);
//     }
// }



// С флагом -e пока не ебу, что делать
// У флага -n нет зеленого цвета
// Нет выделения вхождений красным цветом
// Если -с используется на нескольких файлах то вывод вот такой примерно:
// b.txt:75
// c.txt:5
// a.txt:107
// Так же происходит и с другими флагами, или вовсе без флагов
// Вывод -l не фиолетовый
