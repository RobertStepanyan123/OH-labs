student@DESKTOP-RT4IC58:~/219/lab5/lab5.4$ truncate -s 1 testfile.bin
student@DESKTOP-RT4IC58:~/219/lab5/lab5.4$ gcc -o mmap mmap.c
student@DESKTOP-RT4IC58:~/219/lab5/lab5.4$ ./mmap testfile.bin
Current string=

Attempting to write beyond actual file size to trigger SIGBUS...
student@DESKTOP-RT4IC58:~/219/lab5/lab5.4$