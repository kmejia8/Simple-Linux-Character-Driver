/* Author: Karla Mejia
Linux Character Device Driver
This works on kernel version 6.8.0-85-generic

This code is provided here solely for educational and portfolio purposes.  
No permission is granted to copy, modify, or redistribute this code.  

*/

#include <linux/module.h> // macros + functions for loadable kernel modules
#include <linux/init.h> // for function initialization and cleanup
#include <linux/fs.h> // file operations to interact with VFS
#include <linux/version.h> // adapts code based on running kernel version
#include <linux/device.h> // manages devices and classes
#include <linux/cdev.h> // character device structure and any helpers

static unsigned int major; // major number/integer parameter
static struct class *new_class; // pointer for device class
static struct cdev new_cdev; // char device struct, links file operations to device num

// protypes for functions
int new_open(struct inode *inode, struct file *filp);
int new_release(struct inode *inode, struct file *filp);
ssize_t new_read(struct file *filp, char __user *buf, size_t count, loff_t *offset);
ssize_t new_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset);

// called when driver is opened
int new_open(struct inode *inode, struct file *filp) {
    pr_info("Driver opened.\n"); // message logged to kernel
    return 0;
}

// called when driver is closed
int new_release(struct inode *inode, struct file *filp) {
    pr_info("Driver closed.\n");
    return 0;
}

// handles reading from user space
ssize_t new_read(struct file *filp, char __user *buf, size_t count, loff_t *offset) {
    pr_info("No more data to read.\n");
    return 0;
}

// handles writing from user space
ssize_t new_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset) {
    pr_info("No more data to accept\n");
    return count;
}

// function table that connects functions of driver to kernel file system
struct file_operations new_fops = {
    // tells kernel which function to call when user interacts with /dev
    open: new_open,
    release: new_release,
    read: new_read,
    write: new_write,
};

// when module is loaded, runs on insmod
static int __init new_char_init_module(void) {
    struct device *new_device; // for pointing to device
    int error; // for error codes
    dev_t devt = 0; // device number

    error = alloc_chrdev_region(&devt, 0, 1, "new_char"); // giving driver a number range, 1 in this case
    // logs error and aborts if it can't get a major number
    if (error < 0) {
        pr_err("Can't get major number.\n");
        return error;
    }

    major = MAJOR(devt); // extracts major number

    // message printed upon itilization
    pr_info("Hello. new_char's major number = %d\n", major);


    // creating device class visible in /sys/class
    new_class = class_create("new_char_class");

    // if there was an error, free's device number and aborts operation
    if (IS_ERR(new_class)) {
        pr_err("There was an error creating the new char class.\n"); // logs error
        unregister_chrdev_region(MKDEV(major, 0), 1); // undo allocation
        return PTR_ERR(new_class);
    }

    // initializing char device and linking file operations
    cdev_init(&new_cdev, &new_fops);
    new_cdev.owner = THIS_MODULE;


    // allows users to have access to device
    cdev_add(&new_cdev, devt, 1);

    new_device = device_create(new_class, // class created earlier
                                NULL, // will have no parent device
                                devt, // associates with dev_t (device number)
                                NULL, // no additional data
                                "new-char" // name of the device
    );

    // does clean up if the device creation fails
    if (IS_ERR(new_device)) {
        pr_err("There was an error creating the new char device.\n");
        class_destroy(new_class); // deletes class
        unregister_chrdev_region(devt, 1); // undo allocation
        return -1;
    }

    pr_info("new char module has been loaded!\n");

    return 0;

}

// for module exit, runs on rmmod
static void __exit new_char_cleanup_module(void) {
    unregister_chrdev_region(MKDEV(major, 0), 1);
    device_destroy(new_class, MKDEV(major, 0));
    cdev_del(&new_cdev);
    class_destroy(new_class);

    pr_info("Goodbye. Module has been unloaded.\n");
}

// for unloading and loading in a module
module_init(new_char_init_module);
module_exit(new_char_cleanup_module);

// license tag for module
MODULE_AUTHOR("Karla Mejia");
MODULE_DESCRIPTION("Creating the new-char driver, heavily referenced by dummy-char.c in LDDD");
MODULE_LICENSE("GPL");
