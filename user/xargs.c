#include "kernel/param.h"
#include "kernel/types.h"
#include "user.h"

int fork1(void);

char *gets_strip(char *buf, int max) {
  int i, cc;
  char c;

  for (i = 0; i + 1 < max;) {
    cc = read(0, &c, 1);
    if (cc < 1)
      break;
    buf[i++] = c;
    if (c == '\n' || c == '\r') {
      buf[i-1] = '\0';
      break;
    }
  }
  buf[i] = '\0';
  return buf;
}

int getcmd(char *buf, int nbuf) {
  memset(buf, 0, nbuf);
  gets_strip(buf, nbuf);
  if (buf[0] == 0) // EOF
    return -1;
  return 0;
}

void xargs(int argc, char *args[]) {
  static char buf[100];

  while (getcmd(buf, sizeof(buf)) >= 0) {
    args[argc] = buf;
    if (fork1() == 0) {
      exec(args[0], args);
      fprintf(2, "exec %s failed\n", args[0]);
    }
    wait(0);
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  char *args[MAXARG];

  if (argc < 2) {
    args[0] = "echo";
    xargs(1, args);
    exit(0);
  }

  for (int i = 1; i < argc; ++i) {
    args[i - 1] = argv[i];
  }
  xargs(argc - 1, args);

  exit(0);
}

int fork1(void) {
  int pid;

  pid = fork();
  if (pid == -1) {
    fprintf(2, "fork\n");
    exit(1);
  }
  return pid;
}
