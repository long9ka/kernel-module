obj-m = hooking.o
DIR = /lib/modules/`uname -r`/build
all:
	make -C $(DIR) M=`pwd` modules
clean:
	make -C $(DIR) M=`pwd` clean