#include "kernel/types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: sleep secs\n");
    exit(1);
  }

  if(sleep(atoi(argv[1])) < 0){
    fprintf(2, "sleep: sleep failed\n");
  }

  exit(0);
}
