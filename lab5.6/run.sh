student@DESKTOP-RT4IC58:~/219/lab5/lab5.6$ gcc -o mmap mmap.c
student@DESKTOP-RT4IC58:~/219/lab5/lab5.6$ truncate -s 15 testfile.bin
student@DESKTOP-RT4IC58:~/219/lab5/lab5.6$ ./mmap testfile.bin "HelloPrivate"
Current string=
Copied "HelloPrivate" to private memory (file not modified)
Segmentation fault
student@DESKTOP-RT4IC58:~/219/lab5/lab5.6$ cat testfile.bin
student@DESKTOP-RT4IC58:~/219/lab5/lab5.6$

