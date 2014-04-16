obj-m += timepolice.o

KERN_TREE ?= /usr/src/linux-headers-$(shell uname -r)

all:
	make -C $(KERN_TREE) M=$(PWD) modules

clean:
	make -C $(KERN_TREE) M=$(PWD) clean

install:
	-rmmod timepolice
	insmod timepolice.ko
