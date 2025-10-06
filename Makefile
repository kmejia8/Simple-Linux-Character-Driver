# Karla Mejia

# Makefile used from LDDD Chapter 4 (for dummy-char.c)
obj-m := new-char.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@