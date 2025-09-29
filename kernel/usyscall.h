// kernel/usyscall.h
#ifndef _USYSCALL_H_
#define _USYSCALL_H_

// layout of the shared user syscall page
struct usyscall {
  int pid;
  // add other read-only fields here in future, keep alignment simple
};

#endif // _USYSCALL_H_