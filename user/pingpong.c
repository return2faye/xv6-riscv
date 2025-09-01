#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int p1[2], p2[2];
  char buf[1];

  if (pipe(p1) < 0 || pipe(p2) < 0) {
    fprintf(2, "pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // ---- child ----
    close(p1[1]); // only read p1
    close(p2[0]); // only write p2

    if (read(p1[0], buf, 1) == 1) {
      int me = getpid();
      fprintf(1, "%d: received ping\n", me);
      write(p2[1], "w", 1);
    }

    close(p1[0]);
    close(p2[1]);
    exit(0);
  } else {
    // ---- parent ----
    close(p1[0]); // only write p1
    close(p2[1]); // only read p2

    write(p1[1], "w", 1);

    if (read(p2[0], buf, 1) == 1) {
      int me = getpid();
      fprintf(1, "%d: received pong\n", me);
    }

    close(p1[1]);
    close(p2[0]);
    wait(0);
    exit(0);
  }
}