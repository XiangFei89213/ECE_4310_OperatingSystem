#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_PATH "/dev/mod03_ioctrl"
#define BUFFER_SIZE 100
#define MY_CUSTOM_COMMAND_1 1
#define MY_CUSTOM_COMMAND_2 2

int main() {
    int fd;
    char write_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];

    // Open the character device
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    // Get user input for writing to the device
    printf("Enter data to write to the device: ");
    if (fgets(write_buffer, sizeof(write_buffer), stdin) == NULL) {
        perror("Failed to read user input");
        close(fd);
        return -1;
    }

    // Write data to the device
    printf("Writing to the device: %s\n", write_buffer);
    if (write(fd, write_buffer, sizeof(write_buffer)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return -1;
    }

    int cmd;
    // Perform the custom IOCTL command
    printf("input io command (1/2)\n");
    scanf ("%d",&cmd);
    if (ioctl(fd, cmd, 0) < 0) {
        perror("Failed to perform custom IOCTL command");
        close(fd);
        return -1;
    }

    // Read data from the device
    printf("Reading from the device...\n");
    if (read(fd, read_buffer, BUFFER_SIZE) < 0) {
        perror("Failed to read from the device");
        close(fd);
        return -1;
    }

    printf("Read from the device: %s\n", read_buffer);

    // Close the device
    close(fd);

    return 0;
}