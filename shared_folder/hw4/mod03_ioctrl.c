#include <linux/module.h>
#include <linux/kernel.h>
//file operation structure
#include <linux/fs.h>
#include <linux/kern_levels.h>
//copy_to_user(), copy_from_user()
#include <linux/uaccess.h>

//lience
MODULE_LICENSE("GPL");

#define ECE_BUF_SIZE 4096 //defines the size of the buffer used by the character device
#define MY_CUSTOM_COMMAND_1 1 // for the ioctl function 
#define MY_CUSTOM_COMMAND_2 2

static char ece_buffer[ECE_BUF_SIZE];
int isReg; //registered successfully? 1 = true, 0 = false
int major; // major number?
int ece_offset_w; // write offset
int ece_offset_r; // read offset
int ece_size;

//WRITE data from the user space to the kernel space buffer (ece_buffer): returns number of bytes written
// #1 *fp = pointer to file structure (like a file descriptor), 
// #2 *buf = pointer to user space buffer w data to be written,
// #3 count = number of bytes to be written, 
// #4 loff_t *op = pointer to the file offset, indicating where the write operation should start
static ssize_t ece_write(struct file *fp, const char *buf, size_t count, loff_t *op) 
{
    int ret = 0;
    if (ece_offset_w + count >= ECE_BUF_SIZE)
    {
        printk(KERN_INFO " ECE 4310: Write Overflow. Abort. \n");
        return -1;
    }

    printk(KERN_INFO " ECE 4310: Copy from user. \n");
    ret = copy_from_user(&ece_buffer[ece_offset_w], buf, count);
    if (ret !=0)
    {
        printk(KERN_INFO " ECE 4310: ERR Copy from user. \n");
        return -1;
    }
    ece_offset_w = ece_offset_w + count;

    return count;
} 

// ssize_t : signed size of allocated array
// READ copies data from the kernel space buffer to the user space: returns the number of bytes read
static ssize_t ece_read(struct file *fp, // pointer to file structure (file descriptor)
    char *buf,          /* to fill with data for user*/
    size_t count,       /* how much data to send */
    loff_t *offset)     /* take the data from offset */
{
    int ret;
    if (ece_offset_r + count >= ECE_BUF_SIZE)
    {
        printk(KERN_INFO " ECE 4310: Read Overflow. Abort. \n");
        return -1;
    }
    printk(KERN_INFO " ECE 4310: Copy to user. \n");

    // copy count bytes of data from kernel space to user space 
    // reutrn 0 means success, >0 means those data fail to transfer
    ret = copy_to_user(buf, &ece_buffer[ece_offset_r], count);

    if (ret != 0)
    {
        printk(KERN_INFO " ECE 4310: ERR Copy to user. \n");
        return -1;
    }

    ece_offset_r = ece_offset_r + count;

    return count;
}


static long ece_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
    // Implementation of the ioctl operation
    switch (cmd) {
        case MY_CUSTOM_COMMAND_1:
            // Handle the custom command
            printk(KERN_INFO " ECE 4310: Custom ioctl command 1 received\n");
            // Additional implementation...
            break;
        case MY_CUSTOM_COMMAND_2:
            printk(KERN_INFO " ECE 4310: Custom ioctl command 2 received\n");
        default:
            return -1;  // Not a valid ioctl command
    }

    return 0;  // Success
}


// Defines file operations that are associated with the character device
// file ops = link between the user-space file operations and the corresponding functions within kernel module
static struct file_operations ece_fops =
{
    .read = ece_read, //pointers to file operations
    .write = ece_write,
    .unlocked_ioctl = ece_ioctl,
};

int ece_init(void) //ran when module is inserted

{
    //initializes the global variables and registers the character device
    int ret = 0;
    major = register_chrdev(0, "mod03_ioctrl", &ece_fops); // passign &ece_fops associates file operations structure with the character device
    ece_offset_w = 0;
    ece_offset_r = 0;
    ece_size = 0;

    if (major < 0)
    {
        isReg = 0;
        printk(KERN_INFO " ECE4310: Start FAIL \n");
    }
    else
    {
        isReg = 1;
        printk(KERN_INFO " ECE4310: Start here \n");
    }
    return 0;
}

void ece_end(void) //unregisters device and releases all resources we were using
{
    if (isReg) //if (major >=0)
    {
        unregister_chrdev(major, "mod03_ioctrl");
    }
    printk(KERN_INFO " ECE4310: End here \n");
}

//these two lines make the kernel register these functions
//the functions can be registered in any order(the name has nothing to do with how it is registered)
module_init(ece_init); //module_init = associated with insert mode event: if something is inserteec, execute ece_init
module_exit(ece_end); //module_eit = associated with exit mode event: if something is remove/exited, execute ece_end