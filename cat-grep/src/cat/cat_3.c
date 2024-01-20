#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>

void parse_args(int argc, char *argv[], int *b_flag, int *e_flag, int *n_flag, int *s_flag, int *t_flag, int *a_flag);
FILE *open_file(const char *filename, char *argv[]);
void process_file(FILE *file, int b_flag, int e_flag, int n_flag, int s_flag, int t_flag, int a_flag);
bool process_s_flag(char* buffer, int *last_empty_line, int s_flag);
void process_b_flag(char *buffer, int b_flag, int *line_number);
void process_n_flag(char *buffer, int n_flag, int s_flag, int *line_number, bool is_squeezable);
void process_e_flag(char *buffer, int e_flag);
void process_t_flag(char *buffer, int t_flag);
void process_a_flag(char *buffer, int a_flag);

int main(int argc, char *argv[]) {
    int b_flag = 0, e_flag = 0, n_flag = 0, s_flag = 0, t_flag = 0, a_flag = 0;

    parse_args(argc, argv, &b_flag, &e_flag, &n_flag, &s_flag, &t_flag, &a_flag);

    for (int i = optind; i < argc; i++) {
        FILE *file = open_file(argv[i], argv);
        process_file(file, b_flag, e_flag, n_flag, s_flag, t_flag, a_flag);
        fclose(file);
    }

    return 0;
}

void parse_args(int argc, char *argv[], int *b_flag, int *e_flag, int *n_flag, int *s_flag, int *t_flag, int *a_flag) {
    int flags;
    const char* short_options = "beEnstTA";
    const struct option long_options[] = {
        {"number-nonblank", no_argument, b_flag, 1},
        {"show-all", no_argument, a_flag, 1},
        {"show-ends", no_argument, e_flag, 1},
        {"number", no_argument, n_flag, 1},
        {"squeeze-blank", no_argument, s_flag, 1},
        {"show-tabs", no_argument, t_flag, 1},
        {0, 0, 0, 0} // Завершающий элемент
    };

    while ((flags = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch(flags) {
            case 0: // Флаг был установлен функцией getopt_long.
                break;
            case 'b':
                (*b_flag)++;
                break;
            case 'e':
            case 'E':
                (*e_flag)++;
                break;
            case 'n':
                (*n_flag)++;
                break;
            case 't':
            case 'T':
                (*t_flag)++;
                break;
            case 's':
                (*s_flag)++;
                break;
            case 'A':
                (*a_flag)++;
                break;
            default:
                fprintf(stderr, "Usage: %s [OPTIONS] [FILE]...\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argc == optind) {
        fprintf(stderr, "Usage: %s [OPTIONS] [FILE]...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

FILE *open_file(const char *filename, char *argv[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

void process_file(FILE *file, int b_flag, int e_flag, int n_flag, int s_flag, int t_flag, int a_flag) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    int last_empty_line = 0;
    int line_number = 1;

    while ((getline(&buffer, &buffer_size, file)) != -1) {
        bool is_squeezable = process_s_flag(buffer, &last_empty_line, s_flag);

        if (!is_squeezable || !s_flag) {
            process_b_flag(buffer, b_flag, &line_number);
            process_n_flag(buffer, n_flag, s_flag, &line_number, is_squeezable);
            process_e_flag(buffer, e_flag);
            process_t_flag(buffer, t_flag);
            process_a_flag(buffer, a_flag);

            fprintf(stdout, "%s", buffer);
        }
    }

    free(buffer);
}

bool process_s_flag(char* buffer, int *last_empty_line, int s_flag) {
    int length = strlen(buffer);
    bool is_empty_line = (length <= 1);

    if (s_flag && is_empty_line) {
        if (*last_empty_line) {
            return true; // Эта строка должна быть сжата
        }
        *last_empty_line = 1;
    } else {
        *last_empty_line = 0;
    }
    return false; // Эта строка не сжимается
}

void process_b_flag(char *buffer, int b_flag, int *line_number) {
    int length = strlen(buffer);
    if (b_flag && length > 1) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        sprintf(buffer, "%*d  ", 6, (*line_number)++);
        strcat(buffer, tmp);
        free(tmp);
    }
}

void process_n_flag(char *buffer, int n_flag, int s_flag, int *line_number, bool is_squeezable) {
    if (n_flag) {
        // Увеличиваем номер строки, если строка не подлежит сжатию или если это первая пустая строка в серии
        if (!s_flag || !is_squeezable) {
            char *tmp = strdup(buffer);
            buffer[0] = '\0';
            sprintf(buffer, "%*d  ", 6, (*line_number)++);
            strcat(buffer, tmp);
            free(tmp);
        }
    }
}

void process_e_flag(char *buffer, int e_flag) {
    int length = strlen(buffer);
    if (e_flag) {
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
            sprintf(buffer + length - 1, "%s", "$\n");
        } else {
            sprintf(buffer + length, "%s", "$");
        }
    }
}

void process_t_flag(char *buffer, int t_flag) {
    int length = strlen(buffer);
    if (t_flag) {
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

void process_a_flag(char *buffer, int a_flag){
    if (a_flag) {
        process_e_flag(buffer, 1);
        process_t_flag(buffer, 1);
    }
}
