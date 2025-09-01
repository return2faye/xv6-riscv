#include "kernel/types.h"
#include "user/user.h"

static void primes(int p[2]) __attribute__((noreturn));  

static void 
primes(int p[2]) 
{
    close(p[1]);

    int prime;
    if (read(p[0], &prime, sizeof(prime)) != sizeof(prime)) {
        close(p[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int p2[2];
    if (pipe(p2) < 0) {
        fprintf(2, "pipe failed\n");
        // close read
        close(p[0]);
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        close(p[0]);
        close(p2[0]);
        close(p2[1]);
        exit(1);
    }
    if(pid == 0) {
        // ----- child -----
        primes(p2);
        exit(0);
    } else {
        // ----- parent -----
        close(p2[0]);
        int n;
        while(read(p[0], &n, sizeof(n)) == sizeof(n)) {
            if (n % prime != 0) {
                if (write(p2[1], &n, sizeof(n)) != sizeof(n)) {
                    break;
                }
            }
        }
        close(p2[1]);
        close(p[0]);

        wait(0);
        exit(0);
    }

}

int 
main(void) 
{
    int p[2];
    if (pipe(p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        close(p[0]);
        close(p[1]);
        exit(1);
    }

    if (pid == 0) {
        // ----- child -----
        primes(p);
        exit(0); 
    } else {
        // ----- parent -----
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            if (write(p[1], &i, sizeof(i)) != sizeof(i)) {
                fprintf(2, "write failed\n");
                break;
            };
        }
        close(p[1]);

        wait(0);
        exit(0);
    }
}