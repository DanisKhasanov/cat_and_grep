#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int pattern_search;
  char patterns[4096];
  int count_file;
} Flags;

int parser(int argc, char *argv[], Flags *flag);
int read_flag_f(char *argv[], Flags *flag);
int all_flags(char **argv, Flags *flag);
void read_flag_o(char *tmp_line, regex_t regex, Flags *flag, int str_count,
                 char **argv);

int main(int argc, char *argv[]) {
  Flags flag = {0};
  int res = 0;
  if (argc > 2) {
    parser(argc, argv, &flag);
    if (res == 0) {
      if (flag.pattern_search == 0) {
        strcpy(flag.patterns, argv[optind]);
        optind++;
      }
      if (optind + 1 < argc) {
        flag.count_file = 1;
      }
      while (optind < argc) {
        all_flags(argv, &flag);
        optind++;
      }
    }
  }
  return 0;
}

int parser(int argc, char *argv[], Flags *flag) {
  int res = 0;
  int current_flag;
  while ((current_flag = (getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0))) !=
         -1) {
    switch (current_flag) {
      case 'e':
        flag->e = 1;
        if (flag->pattern_search > 0) {
          strcat(flag->patterns, "|");
        }
        strcat(flag->patterns, optarg);
        flag->pattern_search++;
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        read_flag_f(argv, flag);
        break;
      case 'o':
        flag->o = 1;
        break;
      default:
        fprintf(stderr,
                "s21_grep: invalid option -- �"
                "usage: grep "
                "[-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] [-C[num]]"
                "[-e pattern] [-f file] [--binary-files=value] [--color=when]"
                "[--context[=num]] [--directories=action] [--label]"
                "[--line-buffered] [--null] [pattern] [file ...]");
        break;
    }
  }
  return res;
}

int read_flag_f(char *argv[], Flags *flag) {
  int res = 0;
  int cur;
  size_t length_pattern = 0;
  int flag_f_pattern = 0;
  FILE *fl = fopen(argv[optind - 1], "r");
  if (fl != NULL) {
    if (flag->pattern_search > 0) strcat(flag->patterns, "|");
    while ((cur = fgetc(fl)) != EOF) {
      length_pattern = strlen(flag->patterns);
      if ((cur == '\0') || (cur == '\n')) {
        if (flag_f_pattern == 0) {
          strcat(flag->patterns, "|");
          flag_f_pattern = 1;
        } else {
          strcat(flag->patterns, ".");
        }
      } else {
        flag->patterns[length_pattern] = cur;
        flag_f_pattern = 0;
      }
    }
    flag->count_file++;
  } else {
    res = 1;
  }
  fclose(fl);
  return res;
}

int all_flags(char *argv[], Flags *flag) {
  FILE *file_name = fopen(argv[optind], "r");
  regex_t regex;
  size_t length;
  char *temporary_line = NULL;
  int cflags = REG_EXTENDED;
  int success;
  int count_str = 0;
  int count_success = 0;
  int i;
  if (flag->i) cflags = REG_EXTENDED | REG_ICASE;
  if (file_name != 0) {
    if (regcomp(&regex, flag->patterns, cflags) == 0) {
      while (getline(&temporary_line, &length, file_name) != -1) {
        count_str++;
        if ((flag->o == 1) && (flag->l == 0) && (flag->v == 0) &&
            (flag->c == 0) && (flag->count_file == 1)) {
          read_flag_o(temporary_line, regex, flag, count_str, argv);
        } else {
          if (temporary_line != 0) {
            success = regexec(&regex, temporary_line, 0, NULL, 0);
            if (((success == 0) && (flag->v == 0)) ||
                ((success == 1) && (flag->v == 1))) {
              count_success++;
              i = 0;
              if (flag->c == 0) {
                if (flag->l == 1) {
                  printf("%s\n", argv[optind]);
                  break;
                }
                if (((flag->count_file == 1) && (flag->h == 0))) {
                  printf("%s:", argv[optind]);
                }
                if (flag->n == 1) {
                  printf("%d:", count_str);
                }
                while (temporary_line[i]) {
                  if (temporary_line[i] != '\n')
                    printf("%c", temporary_line[i]);
                  i++;
                }
                printf("\n");
              }
            }
          }
        }
      }
      free(temporary_line);
      temporary_line = NULL;
      length = 0;
    }
    regfree(&regex);
  } else {
    if (!flag->s) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
    }
  }
  if ((flag->c == 1) && (count_success == 0)) {
    if (flag->count_file == 1) {
      if (flag->h == 0) {
        printf("%s:%d\n", argv[optind], count_success);
      } else {
        printf("%d\n", count_success);
      }
    } else {
      if (flag->l == 1) printf("%d\n", count_success);
      if (flag->l == 0) printf("%d\n", count_success);
    }
  }
  if ((flag->c == 1) && (count_success > 0)) {
    if (flag->count_file == 1) {
      if (flag->l == 1) {
        if (flag->h == 0) {
          printf("%s:1\n%s\n", argv[optind], argv[optind]);
        } else if (flag->h == 1) {
          printf("1\n%s\n", argv[optind]);
        }
      } else if (flag->h == 1) {
        printf("%d\n", count_success);
      } else {
        printf("%s:%d\n", argv[optind], count_success);
      }
    } else {
      if (flag->l == 1) printf("1\n%s\n", argv[optind]);
      if (flag->l == 0) printf("%d\n", count_success);
    }
  }
  fclose(file_name);
  return 0;
}

void read_flag_o(char *temporary_line, regex_t regex, Flags *flag,
                 int count_str, char *argv[]) {
  regmatch_t pmatch[1];
  int cur = 0;
  while (regexec(&regex, temporary_line, 1, pmatch, 0)) {
    if (cur == 0) {
      if (flag->count_file == 1) {
        if ((flag->h == 0) && (flag->n == 0)) printf("%s:", argv[optind]);
        if ((flag->h == 0) && (flag->n == 1)) printf("%s:", argv[optind]);
        if ((flag->h == 1) && (flag->n == 1)) printf("%d:", count_str);
        if ((flag->h == 0) && (flag->n == 1)) printf("%d:", count_str);
      } else {
        if (flag->n == 1) printf("%d:", count_str);
      }
    }
    for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
      printf("%c", temporary_line[i]);
      cur++;
    }
    printf("\n");
    temporary_line = temporary_line + pmatch[0].rm_eo;
    cur++;
  }
}
