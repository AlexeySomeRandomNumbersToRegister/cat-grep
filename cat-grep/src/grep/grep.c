#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <getopt.h>
#include <ctype.h>

#define ANSI_COLOR_RED "\033[1;31m"
#define ANSI_COLOR_GREEN "\033[1;32m"
#define ANSI_COLOR_PURPLE "\033[1;35m"
#define ANSI_COLOR_BLUE "\e[0;34m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define colon ":"

void parse_args(int argc, char *argv[], int *e_flag, int *i_flag, int *v_flag, int *c_flag, int *l_flag, int *n_flag, int *h_flag, int *s_flag, int *f_flag, int *o_flag, char **e_arg, char **f_arg);
FILE *open_file(const char *filename, char *argv[], int s_flag);
char* strcasestr(const char* haystack, const char* needle);
void finder(FILE *file, int argc, const char *filename, char** f_flag_lines, int *f_cycle_counter, int *v_f_counter, int num_lines, char *required_data, int v_flag, int i_flag, int c_flag, int n_flag, int l_flag, int h_flag, int o_flag, int file_counter, int f_flag);
void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int o_flag);
void process_i_flag(char *buffer, char *required_data, int i_flag, int *line_number, int n_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag);
void process_n_flag(char *buffer, char *required_data, int n_flag, int *line_number, int i_flag, int c_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag);
void process_o_flag (char *buffer, char *required_data, int o_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int v_flag);
void process_f_flag(char *buffer, int num_lines, char **f_flag_lines, int f_flag, int *line_number, int *counter, int *v_f_counter, int file_counter, int h_flag, const char *filename, int *smth_found, int i_flag, int c_flag, int n_flag, int v_flag, int o_flag);
char** get_data_f_flag(char *filename, int *num_lines, int s_flag, char *argv[]);

int main(int argc, char *argv[]){
    int file_counter = 0;
    int f_cycle_counter = 0;
    int v_f_counter = 0;
    char *required_data = NULL;
    char *e_arg = NULL;
    char *f_arg = NULL;
    char **f_flag_lines;
    int num_lines;
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
        f_flag_lines = get_data_f_flag(f_arg, &num_lines, s_flag, argv); // Массив строк, к которому можно обращаться по индексу [0], [1] ...
            for (int j = optind + 1; j < argc; j++) {
                FILE *file = open_file(argv[j], argv, s_flag);
                    if (argc - optind > 1) {
                        file_counter = 1;
                    }
                finder(file, argc, argv[j], f_flag_lines, &f_cycle_counter, &v_f_counter, num_lines, required_data, v_flag, i_flag, c_flag, n_flag, l_flag, h_flag, o_flag, file_counter, f_flag);
            }
        optind--; // Уменьшаем optind, чтобы пропустить -f и перейти к следующему аргументу
    }
    if (argc - optind > 2) {
        file_counter = 1;
    }
    for (int i = optind + 1; i < argc; i++){
        FILE *file = open_file(argv[i], argv, s_flag);
        finder(file, argc, argv[i], f_flag_lines, &f_cycle_counter, &v_f_counter, num_lines, required_data, v_flag, i_flag, c_flag, n_flag, l_flag, h_flag, o_flag, file_counter, f_flag);
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

void finder(FILE *file, int argc, const char *filename, char** f_flag_lines, int *f_cycle_counter, int *v_f_counter, int num_lines, char *required_data, int v_flag, int i_flag, int c_flag, int n_flag, int l_flag, int h_flag, int o_flag, int file_counter, int f_flag){
    char *buffer = NULL;
    size_t buffer_size = 0;
    int line_number = 0;
    int smth_found = 0;
    int counter = 0;

    while((getline(&buffer, &buffer_size, file)) != -1){
        char *match = buffer;
        char *next_match;
        int match_found = 0;

        
        process_v_flag(buffer, required_data, v_flag, i_flag, &line_number, n_flag, h_flag, &smth_found, file_counter, filename, f_flag, o_flag);
        process_n_flag(buffer, required_data, n_flag, &line_number, i_flag, c_flag, v_flag, h_flag, &smth_found, file_counter, filename, f_flag);
        process_i_flag(buffer, required_data, i_flag, &line_number, n_flag, v_flag, h_flag, &smth_found, file_counter, filename, f_flag);
        process_o_flag(buffer, required_data, o_flag, i_flag, &line_number, n_flag, h_flag, &smth_found, file_counter, filename, f_flag, v_flag);
        process_f_flag(buffer, num_lines, f_flag_lines, f_flag, &line_number, &counter, v_f_counter, file_counter, h_flag, filename, &smth_found, i_flag, c_flag, n_flag, v_flag, o_flag);

        if(!v_flag && !i_flag && !n_flag && !o_flag && !f_flag) {

            char *tmp = strdup(buffer);
            buffer[0] = '\0';
            if(strstr(tmp, required_data) != NULL){
                if (file_counter && !h_flag && !c_flag) {
                    // sprintf(buffer, "%s:", filename);
                    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                strcat(buffer, tmp);
                line_number++;
                smth_found = 1;
            }
            free(tmp);
        }
        if(!c_flag && !l_flag && *f_cycle_counter < (argc - optind - 1)) {
            if (f_flag && v_flag && file_counter && !h_flag && !n_flag && buffer[0] != '\0') {
                // fprintf(stdout, "%s:", filename);
                fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
            if (f_flag && v_flag && n_flag && buffer[0] != '\0') {
                if (file_counter && !h_flag) {
                    // fprintf(stdout, "%s:", filename);
                    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                // fprintf(stdout, "%d:", line_number);
                fprintf(stdout, "%s%d%s%s%s", ANSI_COLOR_GREEN, line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
            if (f_flag || v_flag) {
            fprintf(stdout, "%s", buffer);
            }
            if (o_flag) {
                fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, buffer, ANSI_COLOR_RESET);
            }
            if (!f_flag && !v_flag && !o_flag) {
                while ((next_match = strcasestr(match, required_data)) != NULL) {
                    size_t prefix_len = next_match - match;

                    // Вывод префикса
                    if (prefix_len > 0) {
                        fwrite(match, 1, prefix_len, stdout);
                    }

                    // Выделение найденного текста красным цветом
                    if (!i_flag) {
                        fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, required_data, ANSI_COLOR_RESET);
                    }
                    if (i_flag) {
                        fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, found_str, ANSI_COLOR_RESET);
                    }
                    // Перемещение указателя на следующий символ после найденной подстроки
                    match = next_match + strlen(required_data);

                    match_found = 1;
                }

                // Вывод оставшейся части строки, если не найдено больше совпадений
                if (match_found && *match != '\0') {
                    fputs(match, stdout);
                }
            }
        }
        // Старый вариант печати специально для -f -v
    //     if(f_flag && v_flag && *f_cycle_counter < (argc - optind - 1)) {
    //         if (file_counter && !h_flag) {
    //                 fprintf(stdout, "%s:", filename);
    //             }
    //         fprintf(stdout, "%s", buffer);
    //     }
    counter++;
    }
        if (c_flag && !l_flag && *f_cycle_counter < (argc - optind - 1)) {
            if (file_counter && !h_flag) {
                // fprintf(stdout, "%s:", filename);
                fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
            fprintf(stdout, "%d\n", line_number);
        }
        if (l_flag && smth_found && *f_cycle_counter < (argc - optind - 1)){
            // printf("%s\n", filename);
            printf(ANSI_COLOR_PURPLE "%s\n", filename);
        }
    (*f_cycle_counter)++;
    free(buffer);
}

void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int o_flag){
    if(v_flag && !n_flag && !f_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!i_flag && strstr(tmp, required_data) == NULL) {
            if (file_counter && !h_flag) {
                    // sprintf(buffer, "%s:", filename);
                    sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
            strcat(buffer, tmp);
            (*line_number)++;
            (*smth_found) = 1;
        }
        if (i_flag && strcasestr(tmp, required_data) == NULL) {
            if (file_counter && !h_flag) {
                    // sprintf(buffer, "%s:", filename);
                    sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
            strcat(buffer, tmp);
            (*line_number)++;
            (*smth_found) = 1;
        }
        if (o_flag) {
            exit(EXIT_FAILURE);
        }
        free(tmp);
    }
}

void process_i_flag(char *buffer, char *required_data, int i_flag, int *line_number, int n_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag){
    if(i_flag && !v_flag && !f_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if(strcasestr(tmp, required_data) != NULL) {
            if (file_counter && !h_flag) {
                    // sprintf(buffer, "%s:", filename);
                    sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
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

void process_n_flag(char *buffer, char *required_data, int n_flag, int *line_number, int i_flag, int c_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag) {
    if (n_flag && !f_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!c_flag) {
            (*line_number)++;
        }
        if (!i_flag && !v_flag) {
            if(strstr(tmp, required_data) != NULL) {
                if (file_counter && !h_flag) {
                    // sprintf(buffer, "%s:%d:", filename, *line_number);
                    sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                if (!file_counter) {
                    sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    // sprintf(buffer, "%d:", *line_number);
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
                    // sprintf(buffer, "%s:%d:", filename, *line_number);
                    sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                if (!file_counter) {
                    sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    // sprintf(buffer, "%d:", *line_number);
                }
                // sprintf(buffer, "%d:", *line_number);
                sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
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
                    // sprintf(buffer, "%s:%d:", filename, *line_number);
                    sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                if (!file_counter) {
                    // sprintf(buffer, "%d:", *line_number);
                    sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
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
                    // sprintf(buffer, "%s:%d:", filename, *line_number);
                    sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                if (!file_counter) {
                    // sprintf(buffer, "%d:", *line_number);
                    sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
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

void process_o_flag(char *buffer, char *required_data, int o_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int v_flag) {
    if (o_flag && !f_flag && !v_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!i_flag) {
            if (strstr(tmp, required_data) != NULL) {
                if (file_counter && !h_flag) {
                    if (!n_flag) {
                        // sprintf(buffer, "%s:", filename);
                        sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                    if (n_flag) {
                        // sprintf(buffer, "%s:%d:", filename, *line_number);
                        sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                }
                if (!file_counter) {
                    if(n_flag) {
                        // sprintf(buffer, "%d:", *line_number);
                    sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
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
                        // sprintf(buffer, "%s:", filename);
                        sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                    if (n_flag) {
                        // sprintf(buffer, "%s:%d:", filename, *line_number);
                        sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                }
                if (!file_counter) {
                    if(n_flag) {
                        // sprintf(buffer, "%d:", *line_number);
                        sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
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

void process_f_flag(char *buffer, int num_lines, char **f_flag_lines, int f_flag, int *line_number, int *counter, int *v_f_counter, int file_counter, int h_flag, const char *filename, int *smth_found, int i_flag, int c_flag, int n_flag, int v_flag, int o_flag) {
    if (f_flag) {
        char *tmp = strdup(buffer);
        if (v_flag && o_flag) {
            buffer[0] = '\0';
        }
        if (!c_flag) {
            (*line_number)++;
        }
        if (!i_flag && !o_flag && !v_flag) {
            buffer[0] = '\0';
            for (int i = 0; i < num_lines; i++) {
                if(strstr(tmp, f_flag_lines[i]) != NULL){
                    if (file_counter && !h_flag) {
                        // sprintf(buffer, "%s:", filename);
                        sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                    strcat(buffer, tmp);
                    // (*line_number)++;
                    (*smth_found) = 1;
                }
            }
        }
        if (i_flag && !o_flag && !v_flag) {
            buffer[0] = '\0';
            for (int i = 0; i < num_lines; i++) {
                if(strcasestr(tmp, f_flag_lines[i]) != NULL){
                    if (file_counter && !h_flag) {
                        // sprintf(buffer, "%s:", filename);
                        sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                    strcat(buffer, tmp);
                    // (*line_number)++;
                    (*smth_found) = 1;
                }
            }
        }
        if (n_flag && !i_flag && !v_flag) {
            buffer[0] = '\0';
            for (int i = 0; i < num_lines; i++) {
                if(strstr(tmp, f_flag_lines[i]) != NULL) {
                    if (file_counter && !h_flag) {
                        // sprintf(buffer, "%s:%d:", filename, *line_number);
                        sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                    if (!file_counter) {
                        // sprintf(buffer, "%d:", *line_number);
                        sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                    strcat(buffer, tmp);
                    (*smth_found) = 1;
                    if(c_flag) {
                        (*line_number)++;
                    }
                }
            }
        }
        if (n_flag && i_flag && !v_flag) {
            buffer[0] = '\0';
            for (int i = 0; i < num_lines; i++) {
                if(strcasestr(tmp, f_flag_lines[i]) != NULL) {
                    if (file_counter && !h_flag) {
                        // sprintf(buffer, "%s:%d:", filename, *line_number);
                        sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                    if (!file_counter) {
                        // sprintf(buffer, "%d:", *line_number);
                        sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                    }
                    strcat(buffer, tmp);
                    (*smth_found) = 1;
                    if(c_flag) {
                        (*line_number)++;
                    }
                }
            }
        }
        if (v_flag && n_flag && !i_flag) {
            if (*counter == 0) {
                *v_f_counter = 0;
            }
            (*v_f_counter)++;
            for (int i = 0; i < num_lines; i++) {
                // printf("LINE NUMBER: %ld\n", num_lines);
                if(strstr(tmp, f_flag_lines[i]) != NULL) {
                    buffer[0] = '\0';
                    (*smth_found) = 1;
                    // printf("v_f_counter: %d\n", *v_f_counter);
                }
                (*line_number) = (*v_f_counter);
            }
            // printf("COUNTER: %d\n", *counter);
        }
        if (v_flag && n_flag && i_flag) {
            if (*counter == 0) {
                *v_f_counter = 0;
            }
            (*v_f_counter)++;
            for (int i = 0; i < num_lines; i++) {
                // printf("LINE NUMBER: %ld\n", num_lines);
                if(strcasestr(tmp, f_flag_lines[i]) != NULL) {
                    buffer[0] = '\0';
                    (*smth_found) = 1;
                    // printf("v_f_counter: %d\n", *v_f_counter);
                }
                (*line_number) = (*v_f_counter);
            }
            // printf("COUNTER: %d\n", *counter);
        }
        if (v_flag && !i_flag && !o_flag && !n_flag) {
            for (int i = 0; i < num_lines; i++) {
                if(strstr(tmp, f_flag_lines[i]) != NULL) {
                    buffer[0] = '\0';
                    (*v_f_counter)++;
                    (*smth_found) = 1;
                }
                (*line_number) = num_lines + 1 - (*v_f_counter);
            }
        }
        if (v_flag && i_flag && !o_flag && !n_flag) {
            for (int i = 0; i < num_lines; i++) {
                if(strcasestr(tmp, f_flag_lines[i]) != NULL) {
                    buffer[0] = '\0';
                    (*v_f_counter)++;
                    (*smth_found) = 1;
                }
                (*line_number) = num_lines + 1 - (*v_f_counter);
            }
        }
        if (o_flag && !v_flag) {
            buffer[0] = '\0';
            if (!i_flag) {
                for (int i = 0; i < num_lines; i++) {
                    if (strstr(tmp, f_flag_lines[i]) != NULL) {
                        if (file_counter && !h_flag) {
                            if (!n_flag) {
                                // sprintf(buffer, "%s:", filename);
                                sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                            }
                            if (n_flag) {
                                // sprintf(buffer, "%s:%d:", filename, *line_number);
                                sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                            }
                        }
                        if (!file_counter) {
                            if(n_flag) {
                                // sprintf(buffer, "%d:", *line_number);
                                sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                            }
                        }
                        strcat(buffer, f_flag_lines[i]);
                        strcat(buffer, "\n");
                        (*smth_found) = 1;
                        if (!n_flag) {
                            (*line_number)++;
                        }
                    }
                }
            }
            if (i_flag) {
                for (int i = 0; i < num_lines; i++) {
                    char *found_str = strcasestr(tmp, f_flag_lines[i]);
                    if (found_str != NULL) {
                        if (file_counter && !h_flag) {
                            if (!n_flag) {
                                // sprintf(buffer, "%s:", filename);
                                sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                            }
                            if (n_flag) {
                                // sprintf(buffer, "%s:%d:", filename, *line_number);
                                sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                            }
                        }
                        if (!file_counter) {
                            if(n_flag) {
                                // sprintf(buffer, "%d:", *line_number);
                                sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                            }
                        }
                        strncat(buffer, found_str, strlen(f_flag_lines[i]));
                        strcat(buffer, "\n");
                        (*smth_found) = 1;
                        if (!n_flag) {
                            (*line_number)++;
                        }
                    }
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

char** get_data_f_flag(char *filename, int *num_lines, int s_flag, char *argv[]) {
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

// Нет выделения вхождений красным цветом в -f
// Нет повторного вхождения в строку в -f
// Переставить флаги в функциях вместе
// Попробовать сократить функцию process_n_flag() и process_f_fleg() до 50 строк, чтобы соответствовать принципам структурного программирования

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

// Надо создать удобные файлы для тестирования, чтобы ничего не ломалось

// Проверить на valgrind:
// ./my_grep -i -n -l name result.txt b.txt                                  
// result.txt
// b.txt
// Segmentation fault
// Без -n работает, можно прописать

// При использовании -i выводится то, что искалось, а не то, что нашлось
// При использовании -o, если было найдено несколько вхождений на одной строке, они напишутся только 1 раз
// 
