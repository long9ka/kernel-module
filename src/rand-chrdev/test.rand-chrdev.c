#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define DIR "/dev/rand"

int rand_number;

int main() {
    int fd = open(DIR, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return errno;
    }
    int ret = read(fd, &rand_number, sizeof(int));
    if (ret < 0) {
        perror("Failed to read number");
        return errno;
    }

    printf("Random = %d\n", rand_number);
    return 0;
}