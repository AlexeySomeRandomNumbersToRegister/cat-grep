// ВСЁ ОТЛИЧНО РАБОТАЕТ !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! !!! !!! !!! !!!

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
void print_stdout(char *buffer, int *line_number, int *n_counter, char *next_match, char *match1, int match_found, int argc, const char *filename, char** f_flag_lines, int *f_cycle_counter, int num_lines, int v_flag, int i_flag, int c_flag, int n_flag, int l_flag, int h_flag, int o_flag, int file_counter, int f_flag);
void print_c_l_flags(char *buffer, int *line_number, int smth_found, int argc, const char *filename, int *f_cycle_counter, int c_flag, int l_flag, int h_flag, int file_counter);
void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int reti, regex_t regex);
void process_i_flag(char *buffer, char *required_data, int i_flag, int *line_number, int n_flag, int v_flag, int *smth_found, int f_flag, int reti, regex_t regex, int c_flag);
void process_n_flag(char *buffer, char *required_data, int n_flag, int *line_number, int i_flag, int c_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int reti, regex_t regex);
void process_ni_flag(int i_flag, int v_flag, int c_flag, char *required_data, int reti, regex_t regex, char *tmp, char *buffer, int *smth_found, int *line_number);
void process_nv_flag(int i_flag, int v_flag, int c_flag, char *required_data, int reti, regex_t regex, char *tmp, char *buffer, int *smth_found, int *line_number, int h_flag, const char *filename, int file_counter);
void process_nvi_flag(int i_flag, int v_flag, int c_flag, char *required_data, int reti, regex_t regex, char *tmp, char *buffer, int *smth_found, int *line_number, int h_flag, const char *filename, int file_counter);
void process_o_flag (char *buffer, char *required_data, int o_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int v_flag, int reti, regex_t regex);
void process_f_flag(char *buffer, int num_lines, char **f_flag_lines, int f_flag, int *line_number, int *n_counter, int *v_f_counter, int file_counter, int h_flag, const char *filename, int *smth_found, int i_flag, int c_flag, int n_flag, int v_flag, int o_flag);
void process_fi_flag(int i_flag, int o_flag, int v_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found);
void process_fn_flag(int n_flag, int i_flag, int v_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found);
void process_fni_flag(int n_flag, int i_flag, int v_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found);
void process_fvn_flag(int n_flag, int i_flag, int v_flag, int c_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *n_counter, int *v_f_counter, int *line_number);
void process_fvni_flag(int n_flag, int i_flag, int v_flag, int c_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *n_counter, int *v_f_counter, int *line_number);
void process_fv_flag(int n_flag, int i_flag, int v_flag, int c_flag, int o_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *v_f_counter, int *line_number);
void process_fvi_flag(int n_flag, int i_flag, int v_flag, int c_flag, int o_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *v_f_counter, int *line_number);
void process_fo_flag(int n_flag, int i_flag, int v_flag, int h_flag, int o_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *line_number, const char *filename, int file_counter);
void process_foi_flag(int n_flag, int i_flag, int h_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *line_number, const char *filename, int file_counter);
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
                if (file != NULL) {
                    if (argc - optind > 1) {
                        file_counter = 1;
                    }
                finder(file, argc, argv[j], f_flag_lines, &f_cycle_counter, &v_f_counter, num_lines, required_data, v_flag, i_flag, c_flag, n_flag, l_flag, h_flag, o_flag, file_counter, f_flag);
                fclose(file);
                }
            }
        optind--; // Уменьшаем optind, чтобы пропустить -f и перейти к следующему аргументу
    }
    if (argc - optind > 2) {
        file_counter = 1;
    }
    for (int i = optind + 1; i < argc; i++){
        FILE *file = open_file(argv[i], argv, s_flag);
        if (file != NULL) {
            finder(file, argc, argv[i], f_flag_lines, &f_cycle_counter, &v_f_counter, num_lines, required_data, v_flag, i_flag, c_flag, n_flag, l_flag, h_flag, o_flag, file_counter, f_flag);
            fclose(file);
        }
    }
    if(f_flag) {
        for (int i = 0; i < num_lines; i++) {
            free(f_flag_lines[i]);
        }
        free(f_flag_lines);
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
        return NULL;
    }
    return file;
}

void finder(FILE *file, int argc, const char *filename, char** f_flag_lines, int *f_cycle_counter, int *v_f_counter, int num_lines, char *required_data, int v_flag, int i_flag, int c_flag, int n_flag, int l_flag, int h_flag, int o_flag, int file_counter, int f_flag){
    char *buffer = NULL;
    size_t buffer_size = 0;
    int line_number = 0, smth_found = 0, n_counter = 0, reti;
    while((getline(&buffer, &buffer_size, file)) != -1){
        char *match1 = buffer;
        char *next_match = NULL;
        int match_found = 0, offset = 0, found = 0;
        regex_t regex;
        regmatch_t match;
        process_v_flag(buffer, required_data, v_flag, i_flag, &line_number, n_flag, h_flag, &smth_found, file_counter, filename, f_flag, reti, regex);
        process_i_flag(buffer, required_data, i_flag, &line_number, n_flag, v_flag, &smth_found, f_flag, reti, regex, c_flag);
        process_n_flag(buffer, required_data, n_flag, &line_number, i_flag, c_flag, v_flag, h_flag, &smth_found, file_counter, filename, f_flag, reti, regex);
        process_o_flag(buffer, required_data, o_flag, i_flag, &line_number, n_flag, h_flag, &smth_found, file_counter, filename, f_flag, v_flag, reti, regex);
        process_f_flag(buffer, num_lines, f_flag_lines, f_flag, &line_number, &n_counter, v_f_counter, file_counter, h_flag, filename, &smth_found, i_flag, c_flag, n_flag, v_flag, o_flag);
        if(!v_flag && !o_flag && !f_flag) {
            i_flag ? (reti = regcomp(&regex, required_data, REG_ICASE)) : (reti = regcomp(&regex, required_data, 0)); // Компиляция регулярного выражения
            if (reti != 0) {
                fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            if ((reti = regexec(&regex, buffer + offset, 1, &match, 0)) == 0) {
                if (!n_flag) {
                    line_number++;
                }
                smth_found = 1;
                if (file_counter && !h_flag && !c_flag && !l_flag) {
                    fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                if (n_flag && !c_flag) {
                    fprintf(stdout, "%s%d%s%s%s", ANSI_COLOR_GREEN, line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
            }
            while ((reti = regexec(&regex, buffer + offset, 1, &match, 0)) == 0) { // Поиск и подсветка совпадений в строке
                found = 1;
                if (!c_flag && !l_flag) {
                printf("%.*s", match.rm_so, buffer + offset);  // Вывод до вхождения
                printf("\033[1;31m%.*s\033[0m", match.rm_eo - match.rm_so, buffer + offset + match.rm_so); // Вывод вхождения красным
                }
                offset += match.rm_eo;
                if (match.rm_so == match.rm_eo) {
                    break; // Если найдено только одно вхождение, прерываем цикл
                }
            }
            if (found && !l_flag && !c_flag) {
                printf("%s", buffer + offset); // После окончания цикла, выводим оставшуюся часть строки, если было найдено вхождение
            }
            regfree(&regex);
        }
        print_stdout(buffer, &line_number, &n_counter, next_match, match1, match_found, argc, filename, f_flag_lines, f_cycle_counter, num_lines, v_flag, i_flag, c_flag, n_flag, l_flag, h_flag, o_flag, file_counter, f_flag);
    }
print_c_l_flags(buffer, &line_number, smth_found, argc, filename, f_cycle_counter, c_flag, l_flag, h_flag, file_counter);
}

void print_stdout(char *buffer, int *line_number, int *n_counter, char *next_match, char *match1, int match_found, int argc, const char *filename, char** f_flag_lines, int *f_cycle_counter, int num_lines, int v_flag, int i_flag, int c_flag, int n_flag, int l_flag, int h_flag, int o_flag, int file_counter, int f_flag) {
    if (!c_flag && !l_flag && *f_cycle_counter < (argc - optind - 1)) {
        if (f_flag && buffer[0] != '\0' && !o_flag) {
            if (file_counter && !h_flag) {
                fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
            if (n_flag) {
                fprintf(stdout, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
        }
        if (v_flag && !o_flag) {
            fprintf(stdout, "%s", buffer);
        }
        if (f_flag && !o_flag) {
            for (int i = 0; i < num_lines; i++) {
                while ((next_match = (i_flag ? strcasestr(match1, f_flag_lines[i]) : strstr(match1, f_flag_lines[i]))) != NULL) {
                    size_t prefix_len = next_match - match1;
                    if (prefix_len > 0) {
                        fwrite(match1, 1, prefix_len, stdout);
                    }
                    i_flag ? fprintf(stdout, "%s%.*s%s", ANSI_COLOR_RED, (int)strlen(f_flag_lines[i]), next_match, ANSI_COLOR_RESET) : fprintf(stdout, "%s%.*s%s", ANSI_COLOR_RED, (int)strlen(f_flag_lines[i]), next_match, ANSI_COLOR_RESET); // Перемещение указателя на следующий символ после найденной подстроки
                    match1 = next_match + strlen(f_flag_lines[i]);
                    match_found = 1;
                }
            }
            if (match_found && *match1 != '\0') {
                fputs(match1, stdout);
            }
        }
        if (o_flag && !v_flag && !f_flag) {
            fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, buffer, ANSI_COLOR_RESET);
        }
        if (o_flag && f_flag) {
            fprintf(stdout, "%s%s", ANSI_COLOR_RED, buffer);
        }
    } // Для -f и -c 
    if (f_flag && c_flag && !l_flag && *f_cycle_counter < (argc - optind - 1)) {
        for (int i = 0; i < num_lines; i++) {
            while ((next_match = (i_flag ? strcasestr(match1, f_flag_lines[i]) : strstr(match1, f_flag_lines[i]))) != NULL) {
                match1 = next_match + strlen(f_flag_lines[i]);
                match_found = 1;
            }
        }
        if (match_found && *match1 != '\0') {
            if (!v_flag) {
                (*line_number)++;
            }
        }
    }
    (*n_counter)++;
}

void print_c_l_flags(char *buffer, int *line_number, int smth_found, int argc, const char *filename, int *f_cycle_counter, int c_flag, int l_flag, int h_flag, int file_counter) {
        if (c_flag && !l_flag && *f_cycle_counter < (argc - optind - 1)) {
            if (file_counter && !h_flag) {
                fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
                fprintf(stdout, "%d\n", *line_number);
        }
        if (l_flag && smth_found && *f_cycle_counter < (argc - optind - 1)){
            printf(ANSI_COLOR_PURPLE "%s\n", filename);
        }
    (*f_cycle_counter)++;
    free(buffer);
}

void process_v_flag(char *buffer, char *required_data, int v_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int reti, regex_t regex){
    if(v_flag && !n_flag && !f_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!i_flag) {
            reti = regcomp(&regex, required_data, 0);
            if (reti != 0) {
                fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
                exit(EXIT_FAILURE);
            }
            reti = regexec(&regex, tmp, 0, NULL, 0);
            if (reti != 0) {
                if (file_counter && !h_flag) {
                    sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                strcat(buffer, tmp);
                (*smth_found) = 1;
                (*line_number)++;
            }
        }
        if(i_flag) {
            reti = regcomp(&regex, required_data, REG_ICASE);
            if (reti != 0) {
                fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
                exit(EXIT_FAILURE);
            }
            reti = regexec(&regex, tmp, 0, NULL, 0);
            if (reti != 0) {
                if (file_counter && !h_flag) {
                    sprintf(buffer, "%s%s%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
                }
                strcat(buffer, tmp);
                (*smth_found) = 1;
                (*line_number)++;
            }
        }
        free(tmp);
        regfree(&regex);
    }
}

void process_i_flag(char *buffer, char *required_data, int i_flag, int *line_number, int n_flag, int v_flag, int *smth_found, int f_flag, int reti, regex_t regex, int c_flag){
    if(i_flag && !v_flag && !f_flag && !n_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        reti = regcomp(&regex, required_data, REG_ICASE);
        if (reti != 0) {
            fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
            exit(EXIT_FAILURE);
        }
        reti = regexec(&regex, tmp, 0, NULL, 0);
        if (reti == 0) {
            strcat(buffer, tmp);
            (*smth_found) = 1;
            if(!n_flag && !c_flag) {
                (*line_number)++;
            }
        }
        free(tmp);
        regfree(&regex);
    }
}

void process_n_flag(char *buffer, char *required_data, int n_flag, int *line_number, int i_flag, int c_flag, int v_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int reti, regex_t regex) {
    if (n_flag && !f_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (!c_flag) {
            (*line_number)++;
        }
        if (!i_flag && !v_flag) {
            reti = regcomp(&regex, required_data, 0);
            if (reti != 0) {
                fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
                exit(EXIT_FAILURE);
            }
            reti = regexec(&regex, tmp, 0, NULL, 0);
            if (reti == 0) {
                strcat(buffer, tmp);
                (*smth_found) = 1;
                if(c_flag) {
                    (*line_number)++;
                }
            }
            free(tmp);
            regfree(&regex);
        }
        process_ni_flag(i_flag, v_flag, c_flag, required_data, reti, regex, tmp, buffer, smth_found, line_number);
        process_nv_flag(i_flag, v_flag, c_flag, required_data, reti, regex, tmp, buffer, smth_found, line_number, h_flag, filename, file_counter);
        process_nvi_flag(i_flag, v_flag, c_flag, required_data, reti, regex, tmp, buffer, smth_found, line_number, h_flag, filename, file_counter);
    }
}

void process_ni_flag(int i_flag, int v_flag, int c_flag, char *required_data, int reti, regex_t regex, char *tmp, char *buffer, int *smth_found, int *line_number) {
    if (i_flag && !v_flag) {
        reti = regcomp(&regex, required_data, REG_ICASE);
        if (reti != 0) {
            fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
            exit(EXIT_FAILURE);
        }
        reti = regexec(&regex, tmp, 0, NULL, 0);
        if (reti == 0) {
            strcat(buffer, tmp);
            (*smth_found) = 1;
            if(c_flag) {
                (*line_number)++;
            }
        }
        free(tmp);
        regfree(&regex);
    }
}

void process_nv_flag(int i_flag, int v_flag, int c_flag, char *required_data, int reti, regex_t regex, char *tmp, char *buffer, int *smth_found, int *line_number, int h_flag, const char *filename, int file_counter) {
    if (v_flag && !i_flag) {
        reti = regcomp(&regex, required_data, 0);
        if (reti != 0) {
            fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
            exit(EXIT_FAILURE);
        }
        reti = regexec(&regex, tmp, 0, NULL, 0);
        if (reti != 0) {
            if (file_counter && !h_flag) {
                sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
            if (!file_counter) {
                sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
            strcat(buffer, tmp);
            (*smth_found) = 1;
            if(c_flag) {
                (*line_number)++;
            }
        }
        free(tmp);
        regfree(&regex);
    }
}

void process_nvi_flag(int i_flag, int v_flag, int c_flag, char *required_data, int reti, regex_t regex, char *tmp, char *buffer, int *smth_found, int *line_number, int h_flag, const char *filename, int file_counter) {
    if (v_flag && i_flag) {
        reti = regcomp(&regex, required_data, REG_ICASE);
        if (reti != 0) {
            fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
            exit(EXIT_FAILURE);
        }
        reti = regexec(&regex, tmp, 0, NULL, 0);
        if (reti != 0) {
            if (file_counter && !h_flag) {
                sprintf(buffer, "%s%s%s%s%s%d%s%s%s", ANSI_COLOR_PURPLE, filename, ANSI_COLOR_BLUE, colon, ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
            if (!file_counter) {
                sprintf(buffer, "%s%d%s%s%s", ANSI_COLOR_GREEN, *line_number, ANSI_COLOR_BLUE, colon, ANSI_COLOR_RESET);
            }
            strcat(buffer, tmp);
            (*smth_found) = 1;
            if(c_flag) {
                (*line_number)++;
            }
        }
        free(tmp);
        regfree(&regex);
    }
}

void process_o_flag(char *buffer, char *required_data, int o_flag, int i_flag, int *line_number, int n_flag, int h_flag, int *smth_found, int file_counter, const char *filename, int f_flag, int v_flag, int reti, regex_t regex) {
    if (o_flag && !f_flag && !v_flag) {
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        if (i_flag) {
            reti = regcomp(&regex, required_data, REG_ICASE);
        } else {
            reti = regcomp(&regex, required_data, 0);
        }
        if (reti != 0) {
            fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
            exit(EXIT_FAILURE);
        }
        regmatch_t match;
        size_t offset = 0;
        while (regexec(&regex, tmp + offset, 1, &match, 0) == 0) {
            (*smth_found) = 1;
            if (file_counter && !h_flag) {
                strcat(buffer, ANSI_COLOR_PURPLE);
                strcat(buffer, filename);
                strcat(buffer, ANSI_COLOR_BLUE);
                strcat(buffer, colon);
                strcat(buffer, ANSI_COLOR_RED);
            }
            if (n_flag) {
                strcat (buffer, ANSI_COLOR_GREEN);
                char numberStr[20];
                snprintf(numberStr, sizeof(numberStr), "%d", *line_number);
                strcat (buffer, numberStr);
                strcat (buffer, ANSI_COLOR_BLUE);
                strcat (buffer, colon);
                strcat (buffer, ANSI_COLOR_RED);
            }
            strncat(buffer, tmp + offset + match.rm_so, match.rm_eo - match.rm_so);
            strcat(buffer, "\n");
            offset += match.rm_eo;

            if (!n_flag) {
                (*line_number)++;
            }
        }
        free(tmp);
        regfree(&regex);
    }
}

void process_f_flag(char *buffer, int num_lines, char **f_flag_lines, int f_flag, int *line_number, int *n_counter, int *v_f_counter, int file_counter, int h_flag, const char *filename, int *smth_found, int i_flag, int c_flag, int n_flag, int v_flag, int o_flag) {
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
                    sprintf(buffer, "%c", '\0');
                    strcat(buffer, tmp);
                    (*smth_found) = 1;
                }
            }
        }
        process_fi_flag(i_flag, o_flag, v_flag, num_lines, tmp, f_flag_lines, buffer, smth_found);
        process_fn_flag(n_flag, i_flag, v_flag, num_lines, tmp, f_flag_lines, buffer, smth_found);
        process_fni_flag(n_flag, i_flag, v_flag, num_lines, tmp, f_flag_lines, buffer, smth_found);
        process_fvn_flag(n_flag, i_flag, v_flag, c_flag, num_lines, tmp, f_flag_lines, buffer, smth_found, n_counter, v_f_counter, line_number);
        process_fvni_flag(n_flag, i_flag, v_flag, c_flag, num_lines, tmp, f_flag_lines, buffer, smth_found, n_counter, v_f_counter, line_number);
        process_fv_flag(n_flag, i_flag, v_flag, c_flag, o_flag, num_lines, tmp, f_flag_lines, buffer, smth_found, v_f_counter, line_number);
        process_fvi_flag(n_flag, i_flag, v_flag, c_flag, o_flag, num_lines, tmp, f_flag_lines, buffer, smth_found, v_f_counter, line_number);
        process_fo_flag(n_flag, i_flag, v_flag, h_flag, o_flag, num_lines, tmp, f_flag_lines, buffer, smth_found, line_number, filename, file_counter);
        free(tmp);
    }
}

void process_fi_flag(int i_flag, int o_flag, int v_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found) {
    if (i_flag && !o_flag && !v_flag) {
        buffer[0] = '\0';
        for (int i = 0; i < num_lines; i++) {
            if(strcasestr(tmp, f_flag_lines[i]) != NULL){
                sprintf(buffer, "%c", '\0');
                strcat(buffer, tmp);
                (*smth_found) = 1;
            }
        }
    }
}

void process_fn_flag(int n_flag, int i_flag, int v_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found) {
    if (n_flag && !i_flag && !v_flag) {
        buffer[0] = '\0';
        for (int i = 0; i < num_lines; i++) {
            if(strstr(tmp, f_flag_lines[i]) != NULL) {
                sprintf(buffer, "%c", '\0');
                strcat(buffer, tmp);
                (*smth_found) = 1;
            }
        }
    }
}

void process_fni_flag(int n_flag, int i_flag, int v_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found) {
    if (n_flag && i_flag && !v_flag) {
        buffer[0] = '\0';
        for (int i = 0; i < num_lines; i++) {
            if(strcasestr(tmp, f_flag_lines[i]) != NULL) {
                sprintf(buffer, "%c", '\0');
                strcat(buffer, tmp);
                (*smth_found) = 1;
            }
        }
    }
}

void process_fvn_flag(int n_flag, int i_flag, int v_flag, int c_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *n_counter, int *v_f_counter, int *line_number) {
    if (v_flag && n_flag && !i_flag && !c_flag) {
        if (*n_counter == 0) {
            *v_f_counter = 0;
        }
        (*v_f_counter)++;
        for (int i = 0; i < num_lines; i++) {
            if(strstr(tmp, f_flag_lines[i]) != NULL) {
                buffer[0] = '\0';
                (*smth_found) = 1;
            }
            (*line_number) = (*v_f_counter);
        }
    }
}

void process_fvni_flag(int n_flag, int i_flag, int v_flag, int c_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *n_counter, int *v_f_counter, int *line_number) {
    if (v_flag && n_flag && i_flag && !c_flag) {
        if (*n_counter == 0) {
            *v_f_counter = 0;
        }
        (*v_f_counter)++;
        for (int i = 0; i < num_lines; i++) {
            if(strcasestr(tmp, f_flag_lines[i]) != NULL) {
                buffer[0] = '\0';
                (*smth_found) = 1;
            }
            (*line_number) = (*v_f_counter);
        }
    }
}

void process_fv_flag(int n_flag, int i_flag, int v_flag, int c_flag, int o_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *v_f_counter, int *line_number) {
    if (v_flag && !i_flag && !o_flag && !n_flag && !c_flag) {
        for (int i = 0; i < num_lines; i++) {
            if(strstr(tmp, f_flag_lines[i]) != NULL) {
                buffer[0] = '\0';
                (*v_f_counter)++;
                (*smth_found) = 1;
            }
            (*line_number) = num_lines + 1 - (*v_f_counter);
        }
    }
}

void process_fvi_flag(int n_flag, int i_flag, int v_flag, int c_flag, int o_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *v_f_counter, int *line_number) {
    if (v_flag && i_flag && !o_flag && !n_flag && !c_flag) {
        for (int i = 0; i < num_lines; i++) {
            if(strcasestr(tmp, f_flag_lines[i]) != NULL) {
                buffer[0] = '\0';
                (*v_f_counter)++;
                (*smth_found) = 1;
            }
            (*line_number) = num_lines + 1 - (*v_f_counter);
        }
    }
}

void process_fo_flag(int n_flag, int i_flag, int v_flag, int h_flag, int o_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *line_number, const char *filename, int file_counter) {
    if (o_flag && !v_flag) {
        buffer[0] = '\0';
        if (!i_flag) {
            for (int i = 0; i < num_lines; i++) {
                if (strstr(tmp, f_flag_lines[i]) != NULL) {
                    if (file_counter && !h_flag) {
                        strcat(buffer, ANSI_COLOR_PURPLE);
                        strcat(buffer, filename);
                        strcat(buffer, ANSI_COLOR_BLUE);
                        strcat(buffer, colon);
                        strcat(buffer, ANSI_COLOR_RED);
                    }
                    if (n_flag) {
                        strcat (buffer, ANSI_COLOR_GREEN);
                        char numberStr[20];
                        snprintf(numberStr, sizeof(numberStr), "%d", *line_number);
                        strcat (buffer, numberStr);
                        strcat (buffer, ANSI_COLOR_BLUE);
                        strcat (buffer, colon);
                        strcat (buffer, ANSI_COLOR_RED);
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
        process_foi_flag(n_flag, i_flag, h_flag, num_lines, tmp, f_flag_lines, buffer, smth_found, line_number, filename, file_counter);

    }
}

void process_foi_flag(int n_flag, int i_flag, int h_flag, int num_lines, char *tmp, char **f_flag_lines, char *buffer, int *smth_found, int *line_number, const char *filename, int file_counter) {
    if (i_flag) {
        for (int i = 0; i < num_lines; i++) {
            char *found_str = strcasestr(tmp, f_flag_lines[i]);
            if (found_str != NULL) {
                if (file_counter && !h_flag) {
                    strcat(buffer, ANSI_COLOR_PURPLE);
                    strcat(buffer, filename);
                    strcat(buffer, ANSI_COLOR_BLUE);
                    strcat(buffer, colon);
                    strcat(buffer, ANSI_COLOR_RED);
                }
                if (n_flag) {
                    strcat (buffer, ANSI_COLOR_GREEN);
                    char numberStr[20];
                    snprintf(numberStr, sizeof(numberStr), "%d", *line_number);
                    strcat (buffer, numberStr);
                    strcat (buffer, ANSI_COLOR_BLUE);
                    strcat (buffer, colon);
                    strcat (buffer, ANSI_COLOR_RED);
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

// Переставить флаги в функциях вместе

// Попробовать сократить функцию process_n_flag() и process_f_flag() до 50 строк, чтобы соответствовать принципам структурного программирования

// strcasestr работает только с английским текстом

// -c -f -v не будет работать как надо

// -o -n -f может выкидывать приколы, так как моя логика может искать вхождение, например, букву, внутри другого вхождения, например, слова

// -o -f выводит только 1 найденное на строке вхождение, если встречается несколько одинаковых
