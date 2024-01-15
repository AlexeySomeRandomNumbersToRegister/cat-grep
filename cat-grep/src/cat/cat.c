// Все работает, но без возможности использования нескольких флагов единоразово 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void input(char *a);
FILE *file_name(char *filename, size_t size);
void print_file(FILE *file);
void print_n_file(FILE *file);
void print_b_file(FILE *file);
void print_e_file(FILE *file);
void print_E_file(FILE *file);
void print_s_file(FILE *file);
void print_t_file(FILE *file);
void print_T_file(FILE *file);
void print_A_file(FILE *file);
void print_v_file(FILE *file);
char check_flags(char *flag);

int main() {
  char cat[3];
  input(cat);
  char flag[2];
  char indicator = check_flags(flag);
  ungetc(indicator, stdin);
  size_t size = 10;
  char *filename = (char*)malloc(size*sizeof(char));

  FILE *file = file_name(filename, size);
  print_file(file);
}

void input(char *a) {
  scanf("%s", a);

  if (strcmp("cat", a) != 0) {
    printf("No such command\n");
    exit(EXIT_FAILURE);
  }
}

FILE *file_name(char *filename, size_t size) {
  FILE *file;
  scanf("%s", filename);
  unsigned length = strlen(filename);
  if (length >= size) {
    size += 10;
    filename = (char*) realloc(filename, size*sizeof(char));
    }

  file = fopen(filename, "r");
  if (file == NULL) {
    printf("No such file or directory\n");
    exit(EXIT_FAILURE);
  }
  return file;
}

void print_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;

  while ((getline(&buffer, &bufsize, file)) != -1) {
    printf("%s", buffer);
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void print_n_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;
  int numeration = 1;

  while ((getline(&buffer, &bufsize, file)) != -1) {
    printf("    ");
    printf("%d ", numeration);
    numeration += 1;
    printf("%s", buffer);
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void print_b_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;
  int numeration = 1;

  while ((getline(&buffer, &bufsize, file)) != -1) {
    // Проверка на пустую строку
    if (strlen(buffer) > 1) {  // Учитываем символ новой строки
      printf("    ");
      printf("%d ", numeration);
      numeration += 1;
      printf("%s", buffer);
    }
    if (strlen(buffer) <= 1) {
      printf("\n");
    }
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void print_e_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;
  ssize_t characters_read;

  while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
    for (ssize_t i = 0; i < characters_read; ++i) {
      if (buffer[i] == '\n') {
        printf("$\n");
      } else if (buffer[i] < 32 || buffer[i] >= 127) {
        printf("M-%c", buffer[i] + 64);
      } else {
        putchar(buffer[i]);
      }
    }
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void print_E_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;
  ssize_t characters_read;

  while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
    for (ssize_t i = 0; i < characters_read; ++i) {
      if (buffer[i] == '\n') {
        printf("$\n");
      } else {
        putchar(buffer[i]);
      }
    }
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

// Надо разобраться
void print_s_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;

  int previous_empty_line = 0;  // Флаг предыдущей пустой строки
  while ((getline(&buffer, &bufsize, file)) != -1) {
    int is_empty_line = (strlen(buffer) <= 1);
    if (!is_empty_line || !previous_empty_line) {
      // Если строка не пуста или предыдущая строка не была пустой, выводим
      // текущую строку
      printf("%s", buffer);
    }
    previous_empty_line = is_empty_line;
  }
  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void print_T_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;
  ssize_t characters_read;

  while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
    for (ssize_t i = 0; i < characters_read; ++i) {
      if (buffer[i] == '\t') {
        printf("^I");
      } else {
        putchar(buffer[i]);
      }
    }
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void print_t_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;
  ssize_t characters_read;

  while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
    for (ssize_t i = 0; i < characters_read; ++i) {
      if (buffer[i] == '\t') {
        printf("^I");
      } else if (buffer[i] == '\n') {
        printf("\n");
      } else if (buffer[i] < 32 || buffer[i] >= 127) {
        printf("M-%c", buffer[i] + 64);
      } else {
        putchar(buffer[i]);
      }
    }
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void print_A_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;
  ssize_t characters_read;

  while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
    for (ssize_t i = 0; i < characters_read; ++i) {
      if (buffer[i] == '\t') {
        printf("^I");
      } else if (buffer[i] == '\n') {
        printf("$\n");
      } else if (buffer[i] < 32 || buffer[i] >= 127) {
        printf("M-%c", buffer[i] + 64);
      } else {
        putchar(buffer[i]);
      }
    }
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void print_v_file(FILE *file) {
  char *buffer = NULL;
  size_t bufsize = 0;
  ssize_t characters_read;

  while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
    for (ssize_t i = 0; i < characters_read; ++i) {
      if (buffer[i] == '\n') {
        printf("\n");
      } else if ((buffer[i] < 32 || buffer[i] >= 127) && buffer[i] != '\t') {
        printf("M-%c", buffer[i] + 64);
      } else {
        putchar(buffer[i]);
      }
    }
  }

  free(buffer);
  fclose(file);
  exit(EXIT_SUCCESS);
}

char check_flags(char *flag) {
  int indicator;
  getchar();
  indicator = getchar();
  while (indicator ==
         ' ') {  // Предусмотрение множества пробелов между cat и флагом
    indicator = getchar();
  }
  if (indicator == '-') {
    scanf("%s", flag);
    size_t size = 10;
    char *filename = (char*)malloc(size*sizeof(char));
    FILE *file = file_name(filename, size);
    if (strcmp("n", flag) == 0) {
      print_n_file(file);
    }
    if (strcmp("b", flag) == 0) {
      print_b_file(file);
    }
    if (strcmp("e", flag) == 0) {
      print_e_file(file);
    }
    if (strcmp("E", flag) == 0) {
      print_E_file(file);
    }
    if (strcmp("s", flag) == 0) {
      print_s_file(file);
    }
    if (strcmp("T", flag) == 0) {
      print_T_file(file);
    }
    if (strcmp("t", flag) == 0) {
      print_t_file(file);
    }
    if (strcmp("A", flag) == 0) {
      print_A_file(file);
    }
    if (strcmp("v", flag) == 0) {
      print_v_file(file);
    } else {
      printf("No such flag\n");
      exit(EXIT_FAILURE);
    }
  }
  return indicator;
}







// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// void input(char *a);
// FILE *file_name(char *c);
// void print_file(FILE *file);
// void print_n_file(FILE *file);
// void print_b_file(FILE *file);
// void print_e_file(FILE *file);
// void print_E_file(FILE *file);
// void print_s_file(FILE *file);
// void print_t_file(FILE *file);
// void print_T_file(FILE *file);
// void print_A_file(FILE *file);
// void print_v_file(FILE *file);
// char check_flags(char *flag);

// int main() {
//   char cat[3];
//   input(cat);
//   char flag[2];
//   char indicator = check_flags(flag);
//   ungetc(indicator, stdin);
//   char filename[100];
//   FILE *file = file_name(filename);
//   print_file(file);
// }

// void input(char *a) {
//   scanf("%s", a);

//   if (strcmp("cat", a) != 0) {
//     printf("We need cat\n");
//     exit(EXIT_FAILURE);
//   }
// }

// FILE *file_name(char *c) {
//   FILE *file;
//   scanf("%s", c);

//   file = fopen(c, "r");
//   if (file == NULL) {
//     printf("No such file or directory\n");
//     exit(EXIT_FAILURE);
//   }
//   return file;
// }

// void print_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;

//   while ((getline(&buffer, &bufsize, file)) != -1) {
//     printf("%s", buffer);
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// void print_n_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;
//   int numeration = 1;

//   while ((getline(&buffer, &bufsize, file)) != -1) {
//     printf("    ");
//     printf("%d ", numeration);
//     numeration += 1;
//     printf("%s", buffer);
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// void print_b_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;
//   int numeration = 1;

//   while ((getline(&buffer, &bufsize, file)) != -1) {
//     // Проверка на пустую строку
//     if (strlen(buffer) > 1) {  // Учитываем символ новой строки
//       printf("    ");
//       printf("%d ", numeration);
//       numeration += 1;
//       printf("%s", buffer);
//     }
//     if (strlen(buffer) <= 1) {
//       printf("\n");
//     }
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// void print_e_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;
//   ssize_t characters_read;

//   while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
//     for (ssize_t i = 0; i < characters_read; ++i) {
//       if (buffer[i] == '\n') {
//         printf("$\n");
//       } else if (buffer[i] < 32 || buffer[i] >= 127) {
//         printf("M-%c", buffer[i] + 64);
//       } else {
//         putchar(buffer[i]);
//       }
//     }
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// void print_E_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;
//   ssize_t characters_read;

//   while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
//     for (ssize_t i = 0; i < characters_read; ++i) {
//       if (buffer[i] == '\n') {
//         printf("$\n");
//       } else {
//         putchar(buffer[i]);
//       }
//     }
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// // Надо разобраться
// void print_s_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;

//   int previous_empty_line = 0;  // Флаг предыдущей пустой строки
//   while ((getline(&buffer, &bufsize, file)) != -1) {
//     int is_empty_line = (strlen(buffer) <= 1);
//     if (!is_empty_line || !previous_empty_line) {
//       // Если строка не пуста или предыдущая строка не была пустой, выводим
//       // текущую строку
//       printf("%s", buffer);
//     }
//     previous_empty_line = is_empty_line;
//   }
//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// void print_T_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;
//   ssize_t characters_read;

//   while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
//     for (ssize_t i = 0; i < characters_read; ++i) {
//       if (buffer[i] == '\t') {
//         printf("^I");
//       } else {
//         putchar(buffer[i]);
//       }
//     }
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// void print_t_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;
//   ssize_t characters_read;

//   while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
//     for (ssize_t i = 0; i < characters_read; ++i) {
//       if (buffer[i] == '\t') {
//         printf("^I");
//       } else if (buffer[i] == '\n') {
//         printf("\n");
//       } else if (buffer[i] < 32 || buffer[i] >= 127) {
//         printf("M-%c", buffer[i] + 64);
//       } else {
//         putchar(buffer[i]);
//       }
//     }
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// void print_A_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;
//   ssize_t characters_read;

//   while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
//     for (ssize_t i = 0; i < characters_read; ++i) {
//       if (buffer[i] == '\t') {
//         printf("^I");
//       } else if (buffer[i] == '\n') {
//         printf("$\n");
//       } else if (buffer[i] < 32 || buffer[i] >= 127) {
//         printf("M-%c", buffer[i] + 64);
//       } else {
//         putchar(buffer[i]);
//       }
//     }
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// void print_v_file(FILE *file) {
//   char *buffer = NULL;
//   size_t bufsize = 0;
//   ssize_t characters_read;

//   while ((characters_read = getline(&buffer, &bufsize, file)) != -1) {
//     for (ssize_t i = 0; i < characters_read; ++i) {
//       if (buffer[i] == '\n') {
//         printf("\n");
//       } else if ((buffer[i] < 32 || buffer[i] >= 127) && buffer[i] != '\t') {
//         printf("M-%c", buffer[i] + 64);
//       } else {
//         putchar(buffer[i]);
//       }
//     }
//   }

//   free(buffer);
//   fclose(file);
//   exit(EXIT_SUCCESS);
// }

// char check_flags(char *flag) {
//   int indicator;
//   getchar();
//   indicator = getchar();
//   while (indicator ==
//          ' ') {  // Предусмотрение множества пробелов между cat и флагом
//     indicator = getchar();
//   }
//   if (indicator == '-') {
//     scanf("%s", flag);
//     char filename[100];
//     FILE *file = file_name(filename);
//     if (strcmp("n", flag) == 0) {
//       print_n_file(file);
//     }
//     if (strcmp("b", flag) == 0) {
//       print_b_file(file);
//     }
//     if (strcmp("e", flag) == 0) {
//       print_e_file(file);
//     }
//     if (strcmp("E", flag) == 0) {
//       print_E_file(file);
//     }
//     if (strcmp("s", flag) == 0) {
//       print_s_file(file);
//     }
//     if (strcmp("T", flag) == 0) {
//       print_T_file(file);
//     }
//     if (strcmp("t", flag) == 0) {
//       print_t_file(file);
//     }
//     if (strcmp("A", flag) == 0) {
//       print_A_file(file);
//     }
//     if (strcmp("v", flag) == 0) {
//       print_v_file(file);
//     } else {
//       printf("No such flag\n");
//       exit(EXIT_FAILURE);
//     }
//   }
//   return indicator;
// }