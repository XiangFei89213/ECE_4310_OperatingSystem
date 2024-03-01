#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_PATH "/dev/mod04_ioctrl"
#define BUFFER_SIZE 100 // max amount of characters for a single write command
#define RESET_READ_OFFSET 0 // for the ioctl function
#define RESET_WRITE_OFFSET 1 // for the ioctl function

int main() {
    int fd;
    char write_buffer[BUFFER_SIZE]; 
    char read_buffer[BUFFER_SIZE];

    // Open the character device
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        printf("Failed to open the device");
        return -1;
    }

    // Get user input for writing to the device
    printf("Enter data to write to the device: ");
    if (fgets(write_buffer, sizeof(write_buffer), stdin) == NULL) {
        printf("Failed to read user input");
        close(fd);
        return -1;
    }

    // Write data to the device
    printf("Writing to the device: %s\n", write_buffer);
    if (write(fd, write_buffer, sizeof(write_buffer)) < 0) {
        printf("Failed to write to the device");
        close(fd);
        return -1;
    }

   
    // Read data from the device
    printf("Reading from the device...\n");
    if (read(fd, read_buffer, BUFFER_SIZE) < 0) {
        printf("Failed to read from the device");
        close(fd);
        return -1;
    }

    printf("Read from the device: %s\n", read_buffer);

    int cmd;
     // Perform the custom IOCTL command for read
     printf("input ioctrl(1 or 2)\n");
     printf("1 reset write buffer\n");
     printf("2 reset read buffer\n");
     scanf("%d", &cmd);
     ioctl(fd, cmd, 0);
    // if (ioctl(fd, RESET_READ_OFFSET, 0) < 0) {
    //     printf("Failed to perform custom IOCTL command for read reset.\n");
    //     close(fd);
    //     return -1;
    // }

    // // Perform the custom IOCTL command for write
    // if (ioctl(fd, RESET_WRITE_OFFSET, 0) < 0) {
    //     printf("Failed to perform custom IOCTL command for write reset.\n");
    //     close(fd);
    //     return -1;
    // }

    // Close the device
    close(fd);

    return 0;
}