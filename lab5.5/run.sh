student@DESKTOP-RT4IC58:~/219/lab5/lab5.5$ gcc mmap mmap.c
/usr/bin/ld: cannot find mmap: No such file or directory
collect2: error: ld returned 1 exit status
student@DESKTOP-RT4IC58:~/219/lab5/lab5.5$ gcc -o mmap mmap.c
student@DESKTOP-RT4IC58:~/219/lab5/lab5.5$ truncate -s 1 testfile.bin
student@DESKTOP-RT4IC58:~/219/lab5/lab5.5$ gcc -o mmap mmap.c
student@DESKTOP-RT4IC58:~/219/lab5/lab5.5$ ./mmap testfile.bin "HelloPrivate"
Current string=
Copied "HelloPrivate" to shared memory
student@DESKTOP-RT4IC58:~/219/lab5/lab5.5$ cat testfile.bin
student@DESKTOP-RT4IC58:~/219/lab5/lab5.5$
