#include "kernel/types.h"
#include "user.h"

#define MAGIC_VAL 0xBD

int main(int argc, char *argv[]) {
  int ret, cpid, pid;
  int pfd[2];
  int cfd[2];
  char c;

  ret = pipe(pfd);

  if (ret < 0) {
    fprintf(2, "pingpong: pipe failed\n");
    exit(-1);
  }

  ret = pipe(cfd);

  if (ret < 0) {
    fprintf(2, "pingpong: pipe failed\n");
    exit(-1);
  }

  cpid = fork();

  if (cpid < 0) {
    fprintf(2, "pingpong: fork failed\n");
    exit(-1);
  }

  pid = getpid();
  if (pid < 0) {
    fprintf(2, "pingpong: getpid failed\n");
    exit(-1);
  }

  if (cpid == 0) { // children
    ret = close(pfd[1]);
    if (ret < 0) {
      fprintf(2, "pingpong: close failed\n");
      exit(-1);
    }
    ret = close(cfd[0]);
    if (ret < 0) {
      fprintf(2, "pingpong: close failed\n");
      exit(-1);
    }

    ret = read(pfd[0], &c, 1);
    if (ret < 0) {
      fprintf(2, "pingpong: read failed\n");
      exit(-1);
    }

    printf("%d: received ping\n", pid);

    ret = close(pfd[0]);
    if (ret < 0) {
      fprintf(2, "pingpong: close failed\n");
      exit(-1);
    }

    ret = write(cfd[1], &c, 1);
    if (ret < 0) {
      fprintf(2, "pingpong: write failed\n");
      exit(-1);
    }

    ret = close(cfd[1]);
    if (ret < 0) {
      fprintf(2, "pingpong: close failed\n");
      exit(-1);
    }
  } else { // parent
    ret = close(pfd[0]);
    if (ret < 0) {
      fprintf(2, "pingpong: close failed\n");
      exit(-1);
    }
    ret = close(cfd[1]);
    if (ret < 0) {
      fprintf(2, "pingpong: close failed\n");
      exit(-1);
    }

    c = MAGIC_VAL;
    ret = write(pfd[1], &c, 1);
    if (ret < 0) {
      fprintf(2, "pingpong: write failed\n");
      exit(-1);
    }

    ret = close(pfd[1]);
    if (ret < 0) {
      fprintf(2, "pingpong: close failed\n");
      exit(-1);
    }

    ret = read(cfd[0], &c, 1);
    if (ret < 0) {
      fprintf(2, "pingpong: read failed\n");
      exit(-1);
    }

    if (c != MAGIC_VAL) {
      fprintf(2, "pingpong: value mismatch\n");
      exit(-1);
    }

    printf("%d: received pong\n", pid);

    ret = close(cfd[0]);
    if (ret < 0) {
      fprintf(2, "pingpong: close failed\n");
      exit(-1);
    }
  }

  exit(0);
}
