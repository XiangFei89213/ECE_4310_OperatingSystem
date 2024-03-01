#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kern_levels.h>

// need this to use lincense
MODULE_LICENSE("GPL");

int ece_init(void){
    printk(KERN_INFO "ECE4310: start here\n");
    return 0;
}

void ece_end(void){
    printk(KERN_INFO "ECE4310: end here\n");
}

module_init(ece_init);
module_exit(ece_end);