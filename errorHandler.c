#include <stdio.h>

int handleFileOpenError(long result) {
    if (result < 0) {
        perror("Can't open file");
        return -1;
    }
}

int handleFileWriteErrors(long result) {
    if (result < 0) {
        perror("Can't write to file");
        return -1;
    }
    return 0;
}

int handleReadError(long result) {
    if (result == -1) {
        perror("Can't read input\n");
        return -1;
    }
    return 0;
}

int handleSeekErrors(long result) {
    if (result == -1) {
        perror("Can't handle input\n");
        return -1;
    }
    return 0;
}