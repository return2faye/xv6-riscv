#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/stat.h"
#include "user/user.h"

// 一个简单的用户态测试 pgaccess 的程序
int
main(int argc, char *argv[])
{
  // 分配一块内存，保证页对齐
  int npages = 4;
  char *buf = sbrk(npages * PGSIZE);
  if(buf == (char*)-1){
    printf("sbrk failed\n");
    exit(1);
  }

  unsigned int mask = 0;

  // 第 0 页访问一下
  buf[0] = 1;
  // 第 2 页读，并用掉值
  printf("value at page 2 = %d\n", buf[2 * PGSIZE]);

  // 调用 pgaccess 检查这 4 页
  if(pgaccess(buf, npages, &mask) < 0){
    printf("pgaccess syscall failed\n");
    exit(1);
  }

  printf("pgaccess mask = %x\n", mask);

  // mask 应该是 0101（二进制），即 0x5
  if(mask == 0x5){
    printf("pgaccess test: SUCCESS\n");
  } else {
    printf("pgaccess test: FAILED (mask=%x)\n", mask);
  }

  exit(0);
}