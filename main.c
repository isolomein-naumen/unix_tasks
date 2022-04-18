#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "errorHandler.h"

struct inputData {
    char* file1;
    char* file2;
    int blockSize;
};

struct inputData readParams(int argc, char** argv);
int sparseFile(int fromFD, char* saveTo, int blockSize);

int main (int argc, char *argv[]) {
    struct inputData input = readParams(argc, argv);
//    struct inputData input = { "A", "D", 100 };

    if (input.blockSize == 0 || input.file1 == NULL) {
        printf("Bad args\n");
        return -1;
    }

    if (input.file2 == NULL) {
        return sparseFile(STDIN_FILENO, input.file1, input.blockSize);
    } else {
        int fd = open(input.file1, O_RDONLY);
        if (handleFileOpenError(fd) == -1) {
            return -1;
        }
        return sparseFile(fd, input.file2, input.blockSize);
    }
}

int sparseFile(int fromFD, char* saveTo, int blockSize) {
    char* buf = malloc(blockSize);
    int normalCharsCount;
    int zeroesCount;
    long readRes;
    long tempRes;

    int writeToFD = open(saveTo, O_CREAT | O_RDWR, 128 + 256 + 32 + 16);
    if (handleFileOpenError(writeToFD) == -1) {
        return -1;
    }
    while (1) {
        readRes = read(fromFD, buf, blockSize);
        if (handleReadError(readRes) == -1) {
            close(writeToFD);
            close(fromFD);
            return -1;
        }
        if (readRes == 0 || handleReadError(readRes) == -1) {
            break;
        }

        normalCharsCount = 0;
        zeroesCount = 0;

        int i;
        for (i = 0; i < readRes; i++) {
            if (buf[i] != 0) {
                normalCharsCount++;
            } else {
                if (normalCharsCount != 0) {
                    tempRes = write(writeToFD, buf + (i - normalCharsCount), normalCharsCount);
                    if (handleFileWriteErrors(tempRes) == -1) {
                        close(writeToFD);
                        close(fromFD);
                        return -1;
                    }
                }
                normalCharsCount = 0;
                for (; i < readRes ;i++) {
                    if (buf[i] == 0) {
                        zeroesCount++;
                    } else {
                        i--;
                        break;
                    }
                }
                tempRes = lseek(writeToFD, zeroesCount, SEEK_CUR);
                if (handleSeekErrors(tempRes) == -1) {
                    close(writeToFD);
                    close(fromFD);
                    return -1;
                }
                zeroesCount = 0;
            }
        }
        if (normalCharsCount == readRes || normalCharsCount > 0) {
            tempRes = write(writeToFD, buf + (i - normalCharsCount), normalCharsCount);
            if (handleFileWriteErrors(tempRes) == -1) {
                close(writeToFD);
                close(fromFD);
                return -1;
            }
        }
    }
    close(writeToFD);
    close(fromFD);
    return 0;
}


struct inputData readParams(int argc, char** argv) {
    struct inputData input = { NULL, NULL, 4096 };
    int res;
    while ((res = getopt(argc, argv, "b:")) != -1) {
        switch (res) {
            case 'b':
                input.blockSize = atoi(optarg);
                if (input.blockSize == 0) {
                    printf("Block size must be positive number");
                }
                break;
            default:
                break;
        }
    }

    int positionalArgsCount = argc - optind;
    if (positionalArgsCount == 1) {
        input.file1 = argv[optind];
    } else if (positionalArgsCount == 2) {
        input.file1 = argv[optind];
        input.file2 = argv[++optind];
    } else {
        printf("You have to input destination file name.\n");
    }
    return input;
}

