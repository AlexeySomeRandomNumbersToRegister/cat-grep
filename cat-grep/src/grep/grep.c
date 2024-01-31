#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <getopt.h>
#include <ctype.h>

#define ANSI_COLOR_RED "\033[1;31m"
#define ANSI_COLOR_RESET "\x1b[0m"

void parse_args(int argc, char *argv[], int *e_flag, int *i_flag, int *v_flag, int *c_flag, int *l_flag, int *n_flag, int *h_flag, int *s_flag, int *f_flag, int *o_flag, char **e_arg, char **f_arg);
FILE *open_file(const char *filename, char *argv[], int s_flag);
char* strcasestr(const char* haystack, const char* needle);
void finder(FILE *file, const char *filename, char *required_data, int v_flag, int i_flag, int c_flag, int n_flag, int e_flag, int l_flag, int h_flag, int o_flag, int file_counter, int f_flag);
void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename);
void process_i_flag(char *buffer, char *required_data, int i_flag, int *line_number, int n_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename);
void process_n_flag(char *buffer, char *required_data, int n_flag, int *line_number, int i_flag, int c_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename);
void process_o_flag (char *buffer, char *required_data, int o_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename);
char** process_f_flag(char *filename, size_t *num_lines, int s_flag, char *argv[]);

int main(int argc, char *argv[]){
    int file_counter = 0;
    char *required_data = NULL;
    char *e_arg = NULL;
    char *f_arg = NULL;
    char **f_flag_lines;
    size_t num_lines;
    int e_flag = 0, i_flag = 0, v_flag = 0, c_flag = 0, l_flag = 0, n_flag = 0, h_flag = 0, s_flag = 0, f_flag = 0, o_flag = 0; 
    parse_args(argc, argv, &e_flag, &i_flag, &v_flag, &c_flag, &l_flag, &n_flag, &h_flag, &s_flag, &f_flag, &o_flag, &e_arg, &f_arg);
    if (!e_flag && !f_flag) {
        required_data = argv[optind];
    }
    if (e_flag) {
        required_data = e_arg;
        optind-=1;
    }

    if (f_flag) {
        f_flag_lines = process_f_flag(f_arg, &num_lines, s_flag, argv);

        for (int i = 0; i < num_lines; i++) {
            required_data = f_flag_lines[i];
            printf("Цикл: %d. Строка: %s\n", i, required_data);
            
            for (int j = optind + 1; j < argc; j++) {
                FILE *file = open_file(argv[j], argv, s_flag);
                finder(file, argv[j], required_data, v_flag, i_flag, c_flag, n_flag, e_flag, l_flag, h_flag, o_flag, file_counter, f_flag);
            }
        }

        optind--; // Уменьшаем optind, чтобы пропустить -f и перейти к следующему аргументу
    }

    if (argc - optind > 2) {
        file_counter = 1;
    }
    for (int i = optind + 1; i < argc; i++){
        FILE *file = open_file(argv[i], argv, s_flag);
        finder(file, argv[i], required_data, v_flag, i_flag, c_flag, n_flag, e_flag, l_flag, h_flag, o_flag, file_counter, f_flag);
        fclose(file);
    }
    return 0;
}

void parse_args(int argc, char *argv[], int *e_flag, int *i_flag, int *v_flag, int *c_flag, int *l_flag, int *n_flag, int *h_flag, int *s_flag, int *f_flag, int *o_flag, char **e_arg, char **f_arg){
    int flags;
    while((flags = getopt(argc, argv, "e:ivclnhsf:o")) != -1){
        switch(flags){
            case 'e':
                (*e_flag)++;
                *e_arg = optarg;
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
                *f_arg = optarg;
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

FILE *open_file(const char *filename, char *argv[], int s_flag){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        if (!s_flag) {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], filename);
        }
        exit(EXIT_FAILURE);
    }
    return file;
}

void finder(FILE *file, const char *filename, char *required_data, int v_flag, int i_flag, int c_flag, int n_flag, int e_flag, int l_flag, int h_flag, int o_flag, int file_counter, int f_flag){
    char *buffer = NULL;
    size_t buffer_size = 0;
    int line_number = 0;
    int smth_found = 0;

    while(getline(&buffer, &buffer_size, file) != -1){
        process_v_flag(buffer, required_data, v_flag, i_flag, &line_number, n_flag, h_flag, &smth_found, file_counter, filename);
        process_n_flag(buffer, required_data, n_flag, &line_number, i_flag, c_flag, v_flag, h_flag, &smth_found, file_counter, filename);
        process_i_flag(buffer, required_data, i_flag, &line_number, n_flag, v_flag, h_flag, &smth_found, file_counter, filename);
        process_o_flag(buffer, required_data, o_flag, i_flag, &line_number, n_flag, h_flag, &smth_found, file_counter, filename);

        if(!v_flag && !i_flag && !n_flag && !o_flag && !f_flag) {

            char *tmp = strdup(buffer);
            buffer[0] = '\0';
            if(strstr(tmp, required_data) != NULL){
                if (file_counter && !h_flag) {
                    sprintf(buffer, "%s:", filename);
                }
                strcat(buffer, tmp);
                line_number++;
                smth_found = 1;
            }
            free(tmp);
        }
        if(!c_flag && !l_flag) {
            fprintf(stdout, "%s", buffer);
        }
    }
        if (c_flag && !l_flag) {
            if (file_counter && !h_flag) {
                fprintf(stdout, "%s:", filename);
            }
            fprintf(stdout, "%d\n", line_number);
        }
        if (l_flag && smth_found){
            printf("%s\n", filename);
        }
    free(buffer);
}

void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename){
    if(v_flag && !n_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!i_flag && strstr(tmp, required_data) == NULL) {
            if (file_counter && !h_flag) {
                    sprintf(buffer, "%s:", filename);
                }
            strcat(buffer, tmp);
            (*line_number)++;
            (*smth_found) = 1;
        }
        if (i_flag && strcasestr(tmp, required_data) == NULL) {
            if (file_counter && !h_flag) {
                    sprintf(buffer, "%s:", filename);
                }
            strcat(buffer, tmp);
            (*line_number)++;
            (*smth_found) = 1;
        }
        free(tmp);
    }
}

void process_i_flag(char *buffer, char *required_data, int i_flag, int *line_number, int n_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename){
    if(i_flag && !v_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if(strcasestr(tmp, required_data) != NULL) {
            if (file_counter && !h_flag) {
                    sprintf(buffer, "%s:", filename);
                }
            strcat(buffer, tmp);
            (*smth_found) = 1;
            if (!n_flag) {
                (*line_number)++;
            } 
        }
        free(tmp);
    }
}

void process_n_flag(char *buffer, char *required_data, int n_flag, int *line_number, int i_flag, int c_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename) {
    if (n_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!c_flag) {
            (*line_number)++;
        }
        if (!i_flag && !v_flag) {
            if(strstr(tmp, required_data) != NULL) {
                if (file_counter && !h_flag) {
                    sprintf(buffer, "%s:%d:", filename, *line_number);
                }
                if (!file_counter) {
                    sprintf(buffer, "%d:", *line_number);
                }
                strcat(buffer, tmp);
                (*smth_found) = 1;
                if(c_flag) {
                    (*line_number)++;
                }
            }
        }
        if (i_flag && !v_flag) {
            if(strcasestr(tmp, required_data) != NULL) {
                if (file_counter && !h_flag) {
                    sprintf(buffer, "%s:%d:", filename, *line_number);
                }
                if (!file_counter) {
                    sprintf(buffer, "%d:", *line_number);
                }
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
                if (file_counter && !h_flag) {
                    sprintf(buffer, "%s:%d:", filename, *line_number);
                }
                if (!file_counter) {
                    sprintf(buffer, "%d:", *line_number);
                }
                strcat(buffer, tmp);
                (*smth_found) = 1;
                if(c_flag) {
                    (*line_number)++;
                }
            }
        }
        if (v_flag && i_flag) {
            if(strcasestr(tmp, required_data) == NULL) {
                if (file_counter && !h_flag) {
                    sprintf(buffer, "%s:%d:", filename, *line_number);
                }
                if (!file_counter) {
                    sprintf(buffer, "%d:", *line_number);
                }
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

void process_o_flag(char *buffer, char *required_data, int o_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename) {
    if (o_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!i_flag) {
            if (strstr(tmp, required_data) != NULL) {
                if (file_counter && !h_flag) {
                    if (!n_flag) {
                        sprintf(buffer, "%s:", filename);
                    }
                    if (n_flag) {
                        sprintf(buffer, "%s:%d:", filename, *line_number);
                    }
                }
                if (!file_counter) {
                    if(n_flag) {
                        sprintf(buffer, "%d:", *line_number);
                    }
                }
                strcat(buffer, required_data);
                strcat(buffer, "\n");
                (*smth_found) = 1;
                if (!n_flag) {
                    (*line_number)++;
                }
            }
        }
        if(i_flag) {
            char *found_str = strcasestr(tmp, required_data);
            if (found_str != NULL) {
                if (file_counter && !h_flag) {
                    if (!n_flag) {
                        sprintf(buffer, "%s:", filename);
                    }
                    if (n_flag) {
                        sprintf(buffer, "%s:%d:", filename, *line_number);
                    }
                }
                if (!file_counter) {
                    if(n_flag) {
                        sprintf(buffer, "%d:", *line_number);
                    }
                }
                strncat(buffer, found_str, strlen(required_data));
                strcat(buffer, "\n");
                (*smth_found) = 1;
                if (!n_flag) {
                    (*line_number)++;
                }
            }
        }
        free(tmp); 
    }
}

char *strcasestr(const char *haystack, const char *needle) {
    char *result = NULL;
    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;
        while (tolower((unsigned char)*h) == tolower((unsigned char)*n) && *n) {
            ++h;
            ++n;
        }
        if (!*n) {
            result = (char *)haystack;
            break;
        }
        ++haystack;
    }
    return result;
}

char** process_f_flag(char *filename, size_t *num_lines, int s_flag, char *argv[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        if (!s_flag) {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], filename);
        }
        exit(EXIT_FAILURE);
    }
    char *line = NULL;
    size_t line_size = 0;
    size_t count = 0;
    char **lines = NULL;

    while(getline(&line, &line_size, file) != -1) {
        // Удаляем символ новой строки, если он есть
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        // Добавляем строку в массив
        // count + 1, потому что мы собираемся добавить еще одну строку.
        lines = realloc(lines, (count + 1) * sizeof(char*));
        if (lines == NULL) {
            if (!s_flag) {
                fprintf(stderr, "Error: Memory allocation failed.\n");
            }
            exit(EXIT_FAILURE);
        }
        lines[count] = strdup(line);
        count++;
    }
    fclose(file);
    free(line);
    // Обновляем значение num_lines
    *num_lines = count;
    return lines;
}

// У флага -n нет зеленого цвета
// Нет выделения вхождений красным цветом
// Нет повторного вхождения в строку
// Вывод -l не фиолетовый
// Переставить флаги в функциях вместе
// Попробовать сократить функцию process_n_flag() до 50 строк, чтобы соответствовать принципам структурного программирования

// Пофиксить не проверку последнего файла:
// user@DESKTOP-FPE95AJ:/mnt/c/Users/q/Desktop/School21/SimpleBushUtils (Cat Grep)/C3_SimpleBashUtils-1/src/grep$ ./my_grep -s exit a.txt b.txt o.txt c.txt
// b.txt:  -V, --version             display version information and exit
// b.txt:      --help                display this help text and exit
// b.txt:if any error occurs and -q is not given, the exit status is 2.
// user@DESKTOP-FPE95AJ:/mnt/c/Users/q/Desktop/School21/SimpleBushUtils (Cat Grep)/C3_SimpleBashUtils-1/src/grep$ grep -s exit a.txt b.txt o.txt c.txt
// b.txt:  -V, --version             display version information and exit
// b.txt:      --help                display this help text and exit
// b.txt:if any error occurs and -q is not given, the exit status is 2.
// c.txt:exit gdvffg

// При использовании -о текст должен быть красного цвета
// strcasestr работает только с английским текстом
// флаг -f работает только в одиночку
