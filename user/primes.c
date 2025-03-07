#include "kernel/types.h"
#include "user.h"

void sieve(int recvfd) {
  int num;
  int ret = read(recvfd, &num, sizeof(int));
  if (ret < 0) {
    fprintf(2, "primes: read failed\n");
    exit(-1);
  }

  if (ret == 0) {
    close(recvfd);
    exit(0);
  }

  printf("prime %d\n", num);

  int next_num = 0;
  int fd[2];
  if (pipe(fd) < 0) {
    fprintf(2, "primes: pipe failed\n");
    exit(-1);
  }

  int cpid = fork();

  if (cpid == 0) {
    close(recvfd);
    close(fd[1]);
    sieve(fd[0]);
  } else {
    close(fd[0]);
    while (read(recvfd, &next_num, sizeof(int)) > 0) {
      if (next_num % num) {
        write(fd[1], &next_num, sizeof(int));
      }
    }
    close(recvfd);
    close(fd[1]);
    wait(0);
  }
}

int main(int argc, char *argv[]) {
  int fd[2];
  if (pipe(fd) < 0) {
    fprintf(2, "primes: pipe failed\n");
    exit(-1);
  }

  int cpid = fork();
  if (cpid < 0) {
    fprintf(2, "primes: fork failed\n");
    exit(-1);
  }

  if (cpid == 0) {
    close(fd[1]);
    sieve(fd[0]);
  } else {
    close(fd[0]);
    for (int i = 2; i <= 280; ++i) {
      write(fd[1], &i, sizeof(int));
    }
    close(fd[1]);
    wait(0);
  }
  exit(0);
}
