PAC193x_DIR = $(shell pwd)
KVER = $(shell uname -r)
KSRC = /lib/modules/$(KVER)/build
MODDESTDIR = /lib/modules/$(KVER)/kernel/drivers/iio/adc

CROSS_COMPILE	=

AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump
DEPMOD          = /sbin/depmod
INSTALL			= /usr/bin/install
GREP			= /bin/grep

obj-m += pac193x.o

all:
	modules

modules:
	$(MAKE) -C $(KSRC) M=$(PAC193x_DIR) CC=$(CC) modules

install: modules
	@$(GREP) pac193x.ko /lib/modules/$(shell uname -r)/modules.dep && rm -fr $(MODDESTDIR)/pac193x.ko || echo "No system pac193x.ko file found. Install the new pac193x.ko driver into the system"
	$(INSTALL) -p -m 644 pac193x.ko $(MODDESTDIR)
	@$(DEPMOD) -a
	
uninstall:
	@$(GREP) pac193x.ko /lib/modules/$(shell uname -r)/modules.dep && rm -fr $(MODDESTDIR)/pac193x.ko || echo "Remove the pac193x.ko driver from the system"
	@$(DEPMOD) -a
	
clean:
	$(MAKE) -C $(KSRC) M=$(PAC193x_DIR) CC=$(CC) clean
