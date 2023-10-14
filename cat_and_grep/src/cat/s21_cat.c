#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Flags;
int cat_flags(int argc, char *argv[], Flags *flag);
void read_file(char *argv[], Flags *flag);

int main(int argc, char *argv[]) {
  Flags flag = {0};
  cat_flags(argc, argv, &flag);
  while (optind < argc) {
    read_file(argv, &flag);
    optind++;
  }
  return 0;
}

int cat_flags(int argc, char *argv[], Flags *flag) {
  int current_flag;
  const char *short_flag = "+beEnstTv";
  static struct option long_flag[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  while ((current_flag =
              getopt_long(argc, argv, short_flag, long_flag, NULL)) != -1) {
    switch (current_flag) {
      case 'b':
        flag->b = 1;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 'E':
        flag->e = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':
        flag->t = 1;
        flag->v = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      default:
        printf("usage: s21_cat [-benstuv] [file ...]\n");
        break;
    }
    if (flag->b && flag->n) flag->n = 0;
  }
  return 0;
}

void read_file(char *argv[], Flags *flag) {
  FILE *fp;
  fp = fopen(argv[optind], "r");
  if (fp != NULL) {
    int str_count = 1;
    int empty_line = 0;
    int last = '\n';
    while (!feof(fp)) {
      int c = fgetc(fp);
      if (c == EOF) break;

      if (flag->s && c == '\n' && last == '\n') {
        empty_line++;
        if (empty_line > 1) {
          continue;
        }
      } else {
        empty_line = 0;
      }

      if (last == '\n' && ((flag->b && c != '\n') || flag->n))
        printf("%6d\t", str_count++);

      if (flag->t && c == '\t') {
        printf("^");
        c = 'I';
      }

      if (flag->e && c == '\n') printf("$");

      if (flag->v) {
        if ((c >= 0 && c < 9) || (c > 10 && c < 32) || (c > 126 && c <= 160)) {
          printf("^");
          if (c > 126) {
            c -= 64;
          } else {
            c += 64;
          }
        }
      }
      printf("%c", c);
      last = c;
    }
    fclose(fp);
  } else {
    fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
            argv[optind]);
    exit(1);
  }
}