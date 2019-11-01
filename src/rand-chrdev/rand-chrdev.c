/**
 * @file dev-chrdev.c
 * @author Danh Phi Long
 * @date 1 Nov 2019
 * @see https://github.com/long9ka
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>

#define DEVICE "rand"
#define DEVICE_CLASS "rand-chrdev"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Long9ka");
MODULE_DESCRIPTION("A character device");

static int rand_number;
static int major_number;

static struct class *rand_class = NULL;
static struct device *rand_device = NULL;


// struct file_operations /lib/modules/$(uname -r)/build/include/linux/fs.h
static int dev_open(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static int dev_release(struct inode *, struct file *);

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .release = dev_release
};

static __init int  chrdev_init(void) {
    major_number = register_chrdev(0, DEVICE, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register major_number\n");
        return major_number;
    }

    rand_class = class_create(THIS_MODULE, DEVICE_CLASS);
    if (IS_ERR_VALUE(rand_class)) {
        unregister_chrdev(major_number, DEVICE);
        printk(KERN_ERR "Failed to create device class\n");
        return PTR_ERR(rand_class);
    }

    rand_device = device_create(rand_class, NULL, MKDEV(major_number, 0), NULL, DEVICE);
    if (IS_ERR_VALUE(rand_device)) {
        unregister_chrdev(major_number, DEVICE);
        class_destroy(rand_class);
        printk(KERN_ERR "Failed to create device\n");
        return PTR_ERR(rand_device);
    }
    printk(KERN_INFO "Device created !!!\n");
    return 0;
}

static __exit void chrdev_exit(void) {
    device_destroy(rand_class, MKDEV(major_number, 0));
    unregister_chrdev(major_number, DEVICE);
    class_unregister(rand_class);
    class_destroy(rand_class);
    printk(KERN_INFO "Goodbye !!!\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);

// config file_operations
static int dev_open(struct inode *inodep, struct file *filep) {
  printk(KERN_INFO "%s\n", __FUNCTION__);
  return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t length, loff_t *offset) {
  printk(KERN_INFO "%s\n", __FUNCTION__);
  return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
  printk(KERN_INFO "%s\n", __FUNCTION__);
  return 0;
}