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

    fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* Map file privately: MAP_PRIVATE (changes won't affect the file) */
    addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) { /* No longer need fd */
        perror("close");
        exit(EXIT_FAILURE);
    }

    /* Output at most MEM_SIZE bytes */
    printf("Current string=%.*s\n", MEM_SIZE, addr);

    if (argc > 2) { /* Update contents of region */
        if (strlen(argv[2]) >= MEM_SIZE) {
            fprintf(stderr, "'new-value' too large\n");
            exit(EXIT_FAILURE);
        }

        memset(addr, 0, MEM_SIZE); /* Zero out region */
        strncpy(addr, argv[2], MEM_SIZE - 1);

        /* msync won't change file because MAP_PRIVATE */
        if (msync(addr, MEM_SIZE, MS_SYNC) == -1) {
            perror("msync");
            exit(EXIT_FAILURE);
        }

        printf("Copied \"%s\" to private memory (file not modified)\n", argv[2]);
    }

    /* ---------- Force SIGBUS (optional) ---------- */
    // Uncomment this line to test SIGBUS by writing past end of mapping
    // addr[MEM_SIZE] = 'X';

    /* ---------- Unmap memory ---------- */
    if (munmap(addr, MEM_SIZE) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    /* Accessing addr after munmap: undefined behavior, likely SIGSEGV */
    printf("After munmap (dangling pointer): %.*s\n", MEM_SIZE, addr);

    return 0;
}