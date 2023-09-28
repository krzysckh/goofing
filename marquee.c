#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define clr() printf("\033[1A");

static char *argv0 = NULL;

char **marquee(char *s) {
  int i, len = strlen(s);
  char **ret = malloc(sizeof(char*) * len);

  for (i = 0; i < len + 1; ++i) {
    ret[i] = malloc(len + 1 + 1);
    memset(ret[i] + i, ' ', 1);

    strncpy(ret[i] + i + 1, s, len - i + 1);
    strncpy(ret[i], s + len - i, i);

    ret[i][len + 1] = 0;
  }

  return ret;
}

void usage(void) {
  fprintf(stderr, "usage: %s [-a] [-t timeout] text\n", argv0);
  exit(0);
}

int main(int argc, char **argv) {
  char **S;
  int aflag = 0, ch, len, i = 0;
  float tflag = 0.5;

  argv0 = *argv;

  while ((ch = getopt(argc, argv, "at:")) != -1) {
    switch (ch) {
      case 'a':
        aflag = 1;
        break;
      case 't':
        tflag = (float)atof(optarg);
        break;
      default:
        usage();
    }
  }

  if (!argv[optind])
    usage();

  S = marquee(argv[optind]);
  len = strlen(argv[optind]);

  while (1) {
    puts(S[i]);
    if (aflag)
      clr();

    fflush(stdout);

    i = (i + 1) % (len + 1);

    usleep(tflag * 1000000.f);
  }
}
