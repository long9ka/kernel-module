#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/current.h>

MODULE_LICENSE("GPL");

unsigned long sys_call_table = (unsigned long) 0xc08142b0;

char *path = NULL;

asmlinkage unsigned long (original_open) (const char __user, int, int);
asmlinkage unsigned long (original_write) (unsigned int, const char __user, size_t);

asmlinkage unsigned long our_sys_open(const char* __user file_name, int flags, int mode) { 
	printk("%s open %s\n",current->comm, file_name); 
	return original_open(file_name, flags, mode); 
} 

asmlinkage unsigned long our_sys_write(unsigned int fd, const char __user *buffer, size_t count) {
	if (strcmp("rs:main Q:Reg", current->comm) && strcmp("gnome-terminal", current->comm)) {
		printk("%s write size = %d \n",current->comm, count);  
	}
	return original_write(fd, buffer, count); 
}

int make_rw(unsigned long address) {
	unsigned int level;
	pte_t *pte = lookup_address(address, &level); 
	if(pte->pte &~ _PAGE_RW) 
		pte->pte |= _PAGE_RW; 
	return 0; 
}
 
int make_ro(unsigned long address) {
	unsigned int level; 
	pte_t *pte = lookup_address(address, &level); 
	pte->pte = pte->pte &~ _PAGE_RW;
	return 0; 
}
 
static __init int hook_init(void) { 
	make_rw((unsigned long)sys_call_table);
	original_open = (void*)sys_call_table[__NR_open]; 
	original_write = (void*)sys_call_table[__NR_write]; 
	sys_call_table[__NR_open] = (unsigned long)our_sys_open;
	sys_call_table[__NR_write] = (unsigned long)our_sys_write;
	make_ro((unsigned long)sys_call_table); 
	return 0; 
}
 
static void __exit hook_cleanup(void) { 
	make_rw((unsigned long)sys_call_table); 
	sys_call_table[__NR_open] = (unsigned long)original_open; 
	sys_call_table[__NR_write] = (unsigned long)original_write; 
	make_ro((unsigned long)sys_call_table);
} 

module_init(hook_init); 
module_exit(hook_cleanup);