# new-char: Simple Linux Character Device Driver

A minimal Linux character driver written in C, demonstrating how to register a device, link it to file operations, and interact with the Linux kernel via system calls.

Heavily referenced from `dummy-char.c` in *Linux Device Drivers Development (LDDD)*.



## Overview

This driver registers a simple character device with the Linux kernel and handles basic file operations:

- open() – called when the device file is opened  
- read() – called when user space reads from the device  
- write() – called when user space writes to the device  
- release() – called when the device is closed  

It is designed for learning kernel-space programming, device registration, and how the Linux kernel communicates with user space through /dev interfaces.


## Features

- Logs messages to the kernel log (dmesg)  
- Demonstrates:
  - Dynamic major number allocation (alloc_chrdev_region)
  - Character device registration (cdev_init, cdev_add)
  - Device and class creation (class_create, device_create)
  - File operation structure binding (struct file_operations)
  - Safe cleanup and unregistration during module removal


## Build and Run

### 1. Compile the module
Use *make* to compile the kernel object file:
- Ensure the Makefile and new-char.c are in the same directory

```bash
make
```

This creates the necessary files for the module to function.


### 2. Insert (load) the module
```bash
sudo insmod new-char.ko
```

View kernel log messages:
```bash
sudo dmesg | tail
```

Example output:
```
Hello. new_char's major number = 240
new char module has been loaded!
```
- This is typically shown as the last lines of the log

### 3. Test the driver

**Read from the device**
```bash
sudo cat /dev/new-char
```

Check kernel logs:
```bash
dmesg | tail
```
Expected output:
```
Driver opened.
No more data to read.
Driver closed.
```

**Write to the device**
```bash
echo "hello" | sudo tee /dev/new-char
```

Then check logs again:
```bash
sudo dmesg | tail
```
Expected output:
```
Driver opened.
No more data to accept.
Driver closed.
```

### 4. Remove (unload) the module
```bash
sudo rmmod new-char
sudo dmesg | tail
```

Expected output:
```
Goodbye. Module has been unloaded.
```
