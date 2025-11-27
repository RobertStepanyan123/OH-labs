#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int fd_src, fd_dest;
    char *addr_src, *addr_dest;
    struct stat st;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <src> <dest>\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1) {
        fprintf(stderr, "Source file does not exist: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }


    if (fstat(fd_src, &st) == -1) {
        fprintf(stderr, "fstat error\n");
        exit(EXIT_FAILURE);
    }


    addr_src = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd_src, 0);
    if (addr_src == MAP_FAILED) {
        fprintf(stderr, "mmap src error\n");
        exit(EXIT_FAILURE);
    }

    close(fd_src);


    fd_dest = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd_dest == -1) {
        fprintf(stderr, "Cannot open or create dest file: %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }


    if (ftruncate(fd_dest, st.st_size) == -1) {
        fprintf(stderr, "ftruncate error\n");
        exit(EXIT_FAILURE);
    }


    addr_dest = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dest, 0);
    if (addr_dest == MAP_FAILED) {
        fprintf(stderr, "mmap dest error\n");
        exit(EXIT_FAILURE);
    }


    memcpy(addr_dest, addr_src, st.st_size);

    if (msync(addr_dest, st.st_size, MS_SYNC) == -1) {
        fprintf(stderr, "msync error\n");
        exit(EXIT_FAILURE);
    }

    close(fd_dest);

    printf("Copied %s → %s (%ld bytes)\n", argv[1], argv[2], st.st_size);

    return 0;
}