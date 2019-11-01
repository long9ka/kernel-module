obj-m = rand-chrdev.o

DIR = /lib/modules/`uname -r`/build

all:
	make -C $(DIR) M=`pwd` modules
	$(CC) test.rand-chrdev.c -o test.rand-chrdev
clean:
	make -C $(DIR) M=`pwd` clean
	rm test.rand-chrdev