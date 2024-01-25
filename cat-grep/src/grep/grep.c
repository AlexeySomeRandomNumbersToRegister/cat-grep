#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <getopt.h>

#define ANSI_COLOR_RED "\033[1;31m"
#define ANSI_COLOR_RESET "\x1b[0m"

void parse_args(int argc, char *argv[], int *e_flag, int *i_flag, int *v_flag, int *c_flag, int *l_flag, int *n_flag, int *h_flag, int *s_flag, int *f_flag, int *o_flag);
FILE *open_file(const char *filename, char *argv[]);
void finder(FILE *file, char *required_data);

int main(int argc, char *argv[]){
    int e_flag = 0, i_flag = 0, v_flag = 0, c_flag = 0, l_flag = 0, n_flag = 0, h_flag = 0, s_flag = 0, f_flag = 0, o_flag = 0; 
    parse_args(argc, argv, &e_flag, &i_flag, &v_flag, &c_flag, &l_flag, &n_flag, &h_flag, &s_flag, &f_flag, &o_flag);
    for (int i = optind + 1; i < argc; i++){
        FILE *file = open_file(argv[i], argv);
        finder(file, argv[i-1]);
        fclose(file);
    }
}

void parse_args(int argc, char *argv[], int *e_flag, int *i_flag, int *v_flag, int *c_flag, int *l_flag, int *n_flag, int *h_flag, int *s_flag, int *f_flag, int *o_flag){
    int flags;
    while((flags = getopt(argc, argv, "eivclnhsfo")) != -1){
        switch(flags){
            case 'e':
                (*e_flag)++;
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
// void finder(FILE *file, char *required_data){
//     char *buffer = NULL;
//     size_t buffer_size = 0;

//     while((getline(&buffer, &buffer_size, file))!= -1){
//         if(strstr(buffer, required_data) != 0){
//             fprintf(stdout, "%s", buffer);
//         }
//     }
// }


// РАБОТАЕТ, НАДО РАЗОБРАТЬСЯ
void finder(FILE *file, char *required_data) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    
    while ((getline(&buffer, &buffer_size, file) != -1)) {
        char *match = buffer;
        char *next_match;
        int match_found = 0;

        while ((next_match = strstr(match, required_data)) != NULL) {
            size_t prefix_len = next_match - match;

            // Вывод префикса
            if (prefix_len > 0) {
                fwrite(match, 1, prefix_len, stdout);
            }

            // Выделение найденного текста красным цветом
            fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, required_data, ANSI_COLOR_RESET);

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

