#ifdef USE_MAP_ANON
# define _BSD_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int *addr; /* Pointer to memory region */

    #ifdef USE_MAP_ANON /* Use anonymous mapping */
        addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);  // <-- Changed to MAP_PRIVATE

        if (addr == MAP_FAILED) {
            fprintf(stderr, "mmap error\n");
            exit(EXIT_FAILURE);
        }
    #else /* Map /dev/zero */
        int fd;
        fd = open("/dev/zero", O_RDWR);

        if (fd == -1) {
            fprintf(stderr, "open error\n");
            exit(EXIT_FAILURE);
        }

        addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

        if (addr == MAP_FAILED) {
            fprintf(stderr, "mmap error\n");
            exit(EXIT_FAILURE);
        }

        if (close(fd) == -1) {
            fprintf(stderr, "close error\n");
            exit(EXIT_FAILURE);
        }
    #endif

    *addr = 1; /* Initialize integer in mapped region */

    switch (fork()) { /* Parent and child */
        case -1:
            fprintf(stderr, "fork error\n");
            exit(EXIT_FAILURE);
        case 0: /* Child process */
            printf("Child started, value = %d\n", *addr);
            (*addr)++;  // Modify private mapping
            if (munmap(addr, sizeof(int)) == -1) {
                fprintf(stderr, "munmap error\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        default: /* Parent process */
            if (wait(NULL) == -1) {
                fprintf(stderr, "wait error\n");
                exit(EXIT_FAILURE);
            }

            /* With MAP_PRIVATE, parent does NOT see child's increment */
            printf("In parent, value = %d\n", *addr);

            if (munmap(addr, sizeof(int)) == -1) {
                fprintf(stderr, "munmap error\n");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
    }
}