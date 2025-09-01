#include <kernel/types.h>
#include <user/user.h>
#include <kernel/param.h>

int 
main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(2, "Usage: xargs <command> <args...>\n");
        exit(1);
    }

    char buf[512]; // assign an arbitrary value
    int n = 0;
    char c;

    char* args[MAXARG];
    int i;
    for (i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }
    int base = argc - 1;

    while (read(0, &c, 1) == 1) {
        if (c == '\n') {
            buf[n] = 0;
            if (base > MAXARG) {
                fprintf(2, "xargs: too many args");
                exit(1);
            }
            args[base] = buf;
            args[base+1] = 0;
            
            int pid = fork();
            if (pid == 0) {
                exec(argv[1], args);
                fprintf(2, "xargs: exev failed\n");
                exit(1);
            } else if (pid > 0){
                wait(0);
            } else {
                fprintf(2, "xargs: failed to fork\n");
                exit(1);
            }

            n = 0;
        } else {
            buf[n++] = c;
        }
    }

    exit(0);
}