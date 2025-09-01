#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>
#include <kernel/fs.h>
#include <kernel/fcntl.h>

static char*
fmtname(char *path)
{
  char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--) ;
  return p + 1;
}

void
find(char* path, char* target) 
{
    char buf[512], *p; // use p to add path to buffer
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_DEVICE:
            break;
        case T_FILE:
            if (strcmp(fmtname(path), target) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            // check overflow. 1 for '/'
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(path);
            *p++ = '/';

            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }
                // avoid infinite loops
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                find(buf, target);
            }
            break;
    }
    close(fd);
}

int 
main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}