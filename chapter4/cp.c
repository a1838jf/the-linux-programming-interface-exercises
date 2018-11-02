#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif


int main(int argc, char* argv[]) {
    int readFd, outputFd, openFlags, i, numNull = 0;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE + 1];

    if(argc < 3) {
        fprintf(stderr, "usage %s source file target file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    readFd = open(argv[1], O_RDONLY);
    
    if(-1 == readFd) {
        fprintf(stderr, "cannot open file %s \n", argv[1]);
        exit(EXIT_FAILURE);
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

    outputFd = open(argv[2], openFlags, filePerms);

    if(-1 == outputFd) {
        fprintf(stderr, "cannot open file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    while((numRead = read(readFd, buf, BUF_SIZE)) > 0 ) {
        for(i=0; i<numRead; i++) {
            if(buf[i] == '\0') {
                numNull++;
            } else if(numNull > 0) {
                lseek(outputFd, numNull, SEEK_CUR);
                numNull = 0;
                write(outputFd, &buf[i], 1);
            } else {
                write(outputFd, &buf[i], 1);
            }
        }
    }

    if(-1 == close(outputFd)) {
        fprintf(stderr, "cannot close file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

