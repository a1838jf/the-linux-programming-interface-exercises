#define _FILE_OFFSET_BITS 64 

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    int fd;
    off_t off;

    if(argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s pathname offset\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(-1 == fd) {
        fprintf(stderr, "cannnot open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    off = atoll(argv[2]);

    if(lseek(fd, off, SEEK_SET) == -1) {
        fprintf(stderr, "seek failed");
        exit(EXIT_FAILURE);
    }

    if(write(fd, "test", 4) == -1 ) {
        fprintf(stderr, "write failed");
        exit(EXIT_FAILURE);
    }

    close(fd);

    exit(EXIT_SUCCESS);
}

