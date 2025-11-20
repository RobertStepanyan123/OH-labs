#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#define MEM_SIZE 15

/* SIGBUS handler */
void sigbus_handler(int sig, siginfo_t *si, void *unused) {
    (void)unused;
    fprintf(stderr, "\n*** SIGBUS RECEIVED ***\n");
    fprintf(stderr, "Signal: %d, Fault address: %p\n", sig, si->si_addr);
    exit(1);
}

int main(int argc, char *argv[])
{
    char *addr;
    int fd;

    if (argc < 2) {
        fprintf(stderr, "Usage error: %s file [new-value]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Install SIGBUS handler */
    struct sigaction sa = {0};
    sa.sa_sigaction = sigbus_handler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGBUS, &sa, NULL);

    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) { /* fd no longer needed */
        perror("close");
        exit(EXIT_FAILURE);
    }

    /* Output current content safely */
    printf("Current string=%.*s\n", MEM_SIZE, addr);

    if (argc > 2) { /* Update contents of region */
        if (strlen(argv[2]) >= MEM_SIZE) {
            fprintf(stderr, "'new-value' too large\n");
            exit(EXIT_FAILURE);
        }

        memset(addr, 0, MEM_SIZE); /* Zero out region */
        strncpy(addr, argv[2], MEM_SIZE - 1);

        if (msync(addr, MEM_SIZE, MS_SYNC) == -1) {
            perror("msync");
            exit(EXIT_FAILURE);
        }

        printf("Copied \"%s\" to shared memory\n", argv[2]);
    }

    /* Intentionally write beyond the mapped file (simulate truncated file) */
    printf("\nAttempting to write beyond actual file size to trigger SIGBUS...\n");
    addr[MEM_SIZE] = 'X';  // This should trigger SIGBUS

    /* If SIGBUS does not occur (rare), clean up */
    munmap(addr, MEM_SIZE);
    return 0;
}