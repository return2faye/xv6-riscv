#include "kernel/types.h"
#include "user/user.h"

int main() {
  printf("About to touch NULL...\n");
  int *p = 0;
  *p = 123;   // should cause a page fault
  printf("You should not see this.\n");
  exit(0);
}