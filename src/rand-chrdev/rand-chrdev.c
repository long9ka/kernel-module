#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Long9ka");
MODULE_DESCRIPTION("A character device");

static __init int  chrdev_init(void) {
    printk(KERN_INFO "%s\n", __FUNCTION__);
    return 0;
}

static __exit void chrdev_exit(void) {
    printk(KERN_INFO "%s\n", __FUNCTION__);
}

module_init(chrdev_init);
module_exit(chrdev_exit);