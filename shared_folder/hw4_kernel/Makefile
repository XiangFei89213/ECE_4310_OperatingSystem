
obj-m += test_mod00.o
KDIR = /usr/src/linux-headers-6.2.0-36-generic
CFLAGS = -D_KERNEL_ -DMODULE -I$(KDIR)/include -Wall

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) $(KCONFIG) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
	rm -f *.o