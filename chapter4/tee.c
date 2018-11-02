#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif


int main(int argc, char *argv[]) {
    int outputFd, openFlags, opt, append = 0;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE + 1];

    while((opt = getopt(argc, argv, "a")) != -1) {
        switch(opt) {
            case 'a':
                append = 1;
                break;
            default:
                fprintf(stdout, "error argument %c\n", opt);
                break;
        }
    }

    if(argc > 1) {
        openFlags = O_CREAT | O_WRONLY;
        if(append) {
            openFlags |= O_APPEND;
        } else {
            openFlags |= O_TRUNC;
        }

        filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
        outputFd = open(argv[optind], openFlags, filePerms);

        if(-1 == outputFd) {
            fprintf(stderr, "cannot open file %s\n", argv[optind]);
            exit(EXIT_FAILURE);
        }
    }
    
    while((numRead = read(fileno(stdin), buf, BUF_SIZE)) > 0) {
        if(write(fileno(stdout), buf, numRead) != numRead) {
            fprintf(stderr, "write to stdout error!\n");
            exit(EXIT_FAILURE);
        }
        if(argc > 1) {
            if(write(outputFd, buf, numRead) != numRead) {
                fprintf(stderr, "write to file error!\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if(numRead == -1) {
        fprintf(stderr, "read file error!\n");
        exit(EXIT_FAILURE);
    }

    if(close(outputFd) == -1) {
        fprintf(stderr, "close write file error!\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
