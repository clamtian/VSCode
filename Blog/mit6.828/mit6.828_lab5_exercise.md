# Exercise 1
> `i386_init` identifies the file system environment by passing the type `ENV_TYPE_FS` to your environment creation function, `env_create`. Modify `env_create` in `env.c`, so that it gives the file system environment I/O privilege, but never gives that privilege to any other environment.
>
> Make sure you can start the file environment without causing a General Protection fault. You should pass the "fs i/o" test in make grade.

```Javascript
void
env_create(uint8_t *binary, enum EnvType type)
{
	struct Env *e;
	int res = env_alloc(&e, 0);
	if(res < 0) panic("load 1st env failed!\n");
	e->env_type = type;
	load_icode(e, binary);

	// If this is the file server (type == ENV_TYPE_FS) give it I/O privileges.
	// LAB 5: Your code here.
	if (type == ENV_TYPE_FS) 
		e->env_tf.tf_eflags |= FL_IOPL_MASK;
}
```

## Question

> Do you have to do anything else to ensure that this I/O privilege setting is saved and restored properly when you subsequently switch from one environment to another? Why?

不需要做额外处理。不同进程有自己的Trapframe，互不影响。

# Exercise 2
> Implement the `bc_pgfault` and `flush_block` functions in `fs/bc.c`. `bc_pgfault` is a page fault handler, just like the one your wrote in the previous lab for copy-on-write fork, except that its job is to load pages in from the disk in response to a page fault. When writing this, keep in mind that (1) addr may not be aligned to a block boundary and (2) `ide_read` operates in sectors, not blocks.
>
> The `flush_block` function should write a block out to disk if necessary. `flush_block` shouldn't do anything if the block isn't even in the block cache (that is, the page isn't mapped) or if it's not dirty. We will use the VM hardware to keep track of whether a disk block has been modified since it was last read from or written to disk. To see whether a block needs writing, we can just look to see if the PTE_D "dirty" bit is set in the uvpt entry. (The PTE_D bit is set by the processor in response to a write to that page; see 5.2.4.3 in chapter 5 of the 386 reference manual.) After writing the block to disk, `flush_block` should clear the PTE_D bit using `sys_page_map`.
>
> Use make grade to test your code. Your code should pass "`check_bc`", "`check_super`", and "`check_bitmap`".

实现 `fs/bc.c` 中的 `bc_pgfault` 和 `flush_block`。注意这里`flush_block`中的`sys_page_map`使用的权限用 PTE_SYSCALL。

```javaScript
static void
bc_pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t blockno = ((uint32_t)addr - DISKMAP) / BLKSIZE;
	int r;

	// Check that the fault was within the block cache region
	if (addr < (void*)DISKMAP || addr >= (void*)(DISKMAP + DISKSIZE))
		panic("page fault in FS: eip %08x, va %08x, err %04x",
		      utf->utf_eip, addr, utf->utf_err);

	// Sanity check the block number.
	if (super && blockno >= super->s_nblocks)
		panic("reading non-existent block %08x\n", blockno);

	// Allocate a page in the disk map region, read the contents
	// of the block from the disk into that page.
	// Hint: first round addr to page boundary. fs/ide.c has code to read
	// the disk.
	//
	// LAB 5: you code here:
	addr = (void*)ROUNDDOWN(addr, BLKSIZE);
	if ((r = sys_page_alloc(0, addr, PTE_U | PTE_W | PTE_P)))
		panic("bc_pagefault, sys_page_alloc:%e", r);
	if ((r = ide_read(blockno * BLKSECTS, addr, BLKSECTS)))
		panic("bc_pagefault, ide_read:%e", r);
	// Clear the dirty bit for the disk block page since we just read the
	// block from disk
	if ((r = sys_page_map(0, addr, 0, addr, uvpt[PGNUM(addr)] & PTE_SYSCALL)) < 0)
		panic("in bc_pgfault, sys_page_map: %e", r);
	
	// Check that the block we read was allocated. (exercise for
	// the reader: why do we do this *after* reading the block
	// in?)
	if (bitmap && block_is_free(blockno))
		panic("reading free block %08x\n", blockno);
}
```

# Exercise 3

> Use `free_block` as a model to implement `alloc_block` in `fs/fs.c`, which should find a free disk block in the bitmap, mark it used, and return the number of that block. When you allocate a block, you should immediately flush the changed bitmap block to disk with flush_block, to help file system consistency.
>
> Use make grade to test your code. Your code should now pass "`alloc_block`".

使用`free_block`作为参考实现`alloc_block`。

```javascript
int
alloc_block(void)
{
	int i;
	for (i = 3; i < super->s_nblocks; ++i) {
		if (block_is_free(i)) {
			bitmap[i / 32] &= ~(1 << (i % 32));
			flush_block(diskaddr(i));
			return i;
		}
	}
	
	//panic("alloc_block not implemented");
	return -E_NO_DISK;
}
```

# Exercise 4

>  Implement `file_block_walk` and `file_get_block`. `file_block_walk` maps from a block offset within a file to the pointer for that block in the struct File or the indirect block, very much like what `pgdir_walk` did for page tables. `file_get_block` goes one step further and maps to the actual disk block, allocating a new one if necessary.
>
> Use make grade to test your code. Your code should pass "`file_open`", "`file_get_block`", and "`file_flush/file_truncated/file rewrite`", and "`testfile`".

```javascript
static int
file_block_walk(struct File *f, uint32_t filebno, uint32_t **ppdiskbno, bool alloc)
{
        // LAB 5: Your code here.
	    if (filebno >= NDIRECT + NINDIRECT) return -E_INVAL;
	    if (filebno < NDIRECT) {
			*ppdiskbno = f->f_direct + filebno;
		} else {
			if (!f->f_indirect) {
				if (!alloc) return -E_NOT_FOUND;
				int blockno = alloc_block();
				if (blockno < 0) return -E_NO_DISK;
				f->f_indirect = blockno;
				memset(diskaddr(blockno), 0, BLKSIZE);
				flush_block(diskaddr(blockno));
			}
			*ppdiskbno = (uint32_t *)diskaddr(f->f_indirect) + (filebno - NDIRECT);
		}
		return 0;
}

int
file_get_block(struct File *f, uint32_t filebno, char **blk)
{
        // LAB 5: Your code here.
	    uint32_t *ppdiskbno;
	    int r;
		if ((r = file_block_walk(f, filebno, &ppdiskbno, 1)))
	   		return r;
		if (*ppdiskbno == 0) {
			if ((r = alloc_block()) < 0) return r;
			*ppdiskbno = r;
			memset(diskaddr(r), 0, BLKSIZE);
			flush_block(diskaddr(r));
		}
		*blk = diskaddr(*ppdiskbno);
		return 0;
}
```

# Exercise 5

> Implement `serve_read` in `fs/serv.c`.
>
> `serve_read`'s heavy lifting will be done by the already-implemented `file_read` in `fs/fs.c` (which, in turn, is just a bunch of calls to `file_get_block`). `serve_read` just has to provide the RPC interface for file reading. Look at the comments and code in `serve_set_size` to get a general idea of how the server functions should be structured.
>
> Use make grade to test your code. Your code should pass "`serve_open/file_stat/file_close`" and "`file_read`" for a score of 70/150.

具体内容已经在理论篇中分析过。

```javascript
int
serve_read(envid_t envid, union Fsipc *ipc)
{
	struct Fsreq_read *req = &ipc->read;
	struct Fsret_read *ret = &ipc->readRet;

	if (debug)
		cprintf("serve_read %08x %08x %08x\n", envid, req->req_fileid, req->req_n);

	// Lab 5: Your code here:
	struct OpenFile *o;
	int r;
	if ((r = openfile_lookup(envid, req->req_fileid, &o)) < 0)
		return r;
	if ((r = file_read(o->o_file, ret->ret_buf, ipc->read.req_n, o->o_fd->fd_offset)) < 0)
		return r;
	o->o_fd->fd_offset += r;
	return r;
}
```

# Exercise 6

>  Implement `serve_write` in `fs/serv.c` and `devfile_write` in `lib/file.c`.
>
> Use make grade to test your code. Your code should pass "`file_write`", "`file_read after file_write`", "`open`", and "`large file`" for a score of 90/150.

```javascript
int
serve_write(envid_t envid, struct Fsreq_write *req)
{
	if (debug)
		cprintf("serve_write %08x %08x %08x\n", envid, req->req_fileid, req->req_n);

	// LAB 5: Your code here.
	struct OpenFile *o;
	int r;
	if ((r = openfile_lookup(envid, req->req_fileid, &o)) < 0)
		return r;
	if ((r = file_write(o->o_file, req->req_buf, req->req_n, o->o_fd->fd_offset)) < 0)
		return r;
	o->o_fd->fd_offset += r;
	return r;
}

static ssize_t
devfile_write(struct Fd *fd, const void *buf, size_t n)
{
	fsipcbuf.write.req_fileid = fd->fd_file.id;
    fsipcbuf.write.req_n = MIN(n, PGSIZE);
    memmove(fsipcbuf.write.req_buf, buf, fsipcbuf.write.req_n);
    int r = fsipc(FSREQ_WRITE, NULL);
    return r;
}
```














# Exercise 7

>  Implement the system calls described above in `kern/syscall.c` and make sure `syscall()` calls them. You will need to use various functions in `kern/pmap.c` and `kern/env.c`, particularly `envid2env()`. For now, whenever you call `envid2env()`, pass 1 in the checkperm parameter. Be sure you check for any invalid system call arguments, returning `-E_INVAL` in that case. Test your JOS kernel with `user/dumbfork` and make sure it works before proceeding.

实现 `sys_exofork`，`sys_env_set_status`，`sys_page_alloc`，`sys_page_map`，`sys_page_unmap` 这几个系统调用，参照提示依次完成，别忘了在`syscall()`函数中加入对应的系统调用分发代码，最后修改 `kern/init.c` 中加载的用户程序为 `user_dumbfork`即可开始测试。

## `sys_exofork()`

```javascript
static envid_t
sys_exofork(void) 
{
	struct Env *e;
	int res = env_alloc(&e, curenv->env_id);
	if(res > 0) return res;
	e->env_status = ENV_NOT_RUNNABLE;
	e->env_tf = curenv->env_tf;
	e->env_tf.tf_regs.reg_eax = 0;
	return e->env_id;
}
```

## `sys_env_set_status()`

```javascript
static int
sys_env_set_status(envid_t envid, int status)
{
	if(status != ENV_RUNNABLE && status != ENV_NOT_RUNNABLE) return -E_INVAL;

	struct Env* e;
	int res = envid2env(envid, &e, 1);
	if(res < 0) return res;
	e->env_status = status;
	return 0;
}
```

## `sys_page_alloc()`

```javascript
static int
sys_page_alloc(envid_t envid, void *va, int perm)
{
    struct Env *env;
    struct PageInfo *pp;

    if (envid2env(envid, &env, 1) < 0)
            return -E_BAD_ENV;
    if ((uintptr_t)va >= UTOP || PGOFF(va))
            return -E_INVAL;
    if ((perm & PTE_U) == 0 || (perm & PTE_P) == 0)
            return -E_INVAL;
    if ((perm & ~(PTE_U | PTE_P | PTE_W | PTE_AVAIL)) != 0)
            return -E_INVAL;
    if ((pp = page_alloc(ALLOC_ZERO)) == NULL)
            return -E_NO_MEM;
    if (page_insert(env->env_pgdir, pp, va, perm) < 0) {
            page_free(pp);
            return -E_NO_MEM;
    }
    return 0;
}
```

## `sys_page_map()`

```javascript
static int
sys_page_map(envid_t srcenvid, void *srcva,
	     envid_t dstenvid, void *dstva, int perm)
{
	struct Env *srcenv, *dstenv;
    struct PageInfo *pp;
    pte_t *pte;

    if (envid2env(srcenvid, &srcenv, 1) < 0 || envid2env(dstenvid, &dstenv, 1) < 0)
        return -E_BAD_ENV;
    if ((uintptr_t)srcva >= UTOP || PGOFF(srcva) || (uintptr_t)dstva >= UTOP || PGOFF(dstva))
        return -E_INVAL;
    if ((perm & PTE_U) == 0 || (perm & PTE_P) == 0 || (perm & ~PTE_SYSCALL) != 0)
        return -E_INVAL;
    if ((pp = page_lookup(srcenv->env_pgdir, srcva, &pte)) == NULL)
        return -E_INVAL;
    if ((perm & PTE_W) && (*pte & PTE_W) == 0)
        return -E_INVAL;
    if (page_insert(dstenv->env_pgdir, pp, dstva, perm) < 0)
        return -E_NO_MEM;
    return 0;
}
```

## `sys_page_unmap()`

```javascript
static int
sys_page_unmap(envid_t envid, void *va)
{
	struct Env *env;

    if (envid2env(envid, &env, 1) < 0)
        return -E_BAD_ENV;
    if ((uintptr_t)va >= UTOP || PGOFF(va))
        return -E_INVAL;

    page_remove(env->env_pgdir, va);
    return 0;
}
```

最后在`syscall()`函数中加入对应的系统调用分发代码：

```javascript
	 ...
    case SYS_exofork:
        return sys_exofork();
    case SYS_env_set_status:
        return sys_env_set_status(a1, a2);
    case SYS_page_alloc:
        return sys_page_alloc(a1, (void *)a2, a3);
    case SYS_page_map:
        return sys_page_map(a1, (void*)a2, a3, (void*)a4, a5);
    case SYS_page_unmap:
        return sys_page_unmap(a1, (void *)a2);
    ...
```

完成后运行`make grade`，应该可以看到 Part A得分为5分。

# Exercise 8

> Implement the `sys_env_set_pgfault_upcall` system call. Be sure to enable permission checking when looking up the environment ID of the target environment, since this is a "dangerous" system call.

设置`envid`这个用户环境的页面错误处理函数为`func`，我们在Env结构体中添加了`env_pgfault_upcall`这个变量来记录该信息。

```javascript
static int
sys_env_set_pgfault_upcall(envid_t envid, void *func)
{
	struct Env* e;
	int res = envid2env(envid, &e, 1);
	if(res < 0) {
		cprintf("up here");
		return res;
		
	}
	e->env_pgfault_upcall = func;
	return 0;
}
```

# Exercise 9

>  Implement the code in `page_fault_handler` in `kern/trap.c` required to dispatch page faults to the user-mode handler. Be sure to take appropriate precautions when writing into the exception stack. (What happens if the user environment runs out of space on the exception stack?)

在`page_fault_handler`完成用户页面错误的处理，主要是切换堆栈到异常栈，并设置异常栈内容，最后设置EIP为页面错误处理函数的地址，切回用户态执行页面错误处理函数。注意嵌套页错误的处理，嵌套页错误时，需要保留4字节用于设置EIP。

```javascript
void
page_fault_handler(struct Trapframe *tf)
{
	uint32_t fault_va;

	fault_va = rcr2();

	if(!(tf->tf_cs && 0x01)) panic("kernel-mode page fault, fault address %d\n", fault_va);

	if(curenv->env_pgfault_upcall){
		struct UTrapframe *utf;
		if(tf->tf_esp >=  UXSTACKTOP-PGSIZE && tf->tf_esp < UXSTACKTOP){
			utf = (struct UTrapframe *)(tf->tf_esp - sizeof(struct UTrapframe) - 4);
		}else {
			utf = (struct UTrapframe *)(UXSTACKTOP- sizeof(struct UTrapframe));
		}
		user_mem_assert(curenv, utf, 1, PTE_W);

		utf->utf_fault_va = fault_va;
		utf->utf_err = tf->tf_err;
		utf->utf_regs = tf->tf_regs;
		utf->utf_eip = tf->tf_eip;
		utf->utf_eflags = tf->tf_eflags;
		utf->utf_esp = tf->tf_esp;

		curenv->env_tf.tf_eip = (uintptr_t)curenv->env_pgfault_upcall;
		curenv->env_tf.tf_esp = (uintptr_t)utf;
		env_run(curenv);
	}

	cprintf("[%08x] user fault va %08x ip %08x\n",
		curenv->env_id, fault_va, tf->tf_eip);
	print_trapframe(tf);
	env_destroy(curenv);
}
```

如果异常不停递归就可能耗尽异常堆栈的空间，在`page_fault_handler()`里的`user_mem_asser()`会检查是否overflow，如果是就会直接销毁这个env。

# Exercise 10

> Implement the `_pgfault_upcall` routine in `lib/pfentry.S`. The interesting part is returning to the original point in the user code that caused the page fault. You'll return directly there, without going back through the kernel. The hard part is simultaneously switching stacks and re-loading the EIP.

这部分已经在理论部分分析过了。

```javascript
.text
.globl _pgfault_upcall
_pgfault_upcall:
	// Call the C page fault handler.
	pushl %esp			// function argument: pointer to UTF
	movl _pgfault_handler, %eax
	call *%eax			
	addl $4, %esp			// pop function argument

 	// LAB 4: Your code here.
    movl 0x28(%esp), %ebx  # trap-time eip
    subl $0x4, 0x30(%esp)  # trap-time esp minus 4
    movl 0x30(%esp), %eax 
    movl %ebx, (%eax)      # trap-time esp store trap-time eip
    addl $0x8, %esp 

    // Restore the trap-time registers.  After you do this, you
    // can no longer modify any general-purpose registers.
    // LAB 4: Your code here.
    popal

    // Restore eflags from the stack.  After you do this, you can
    // no longer use arithmetic operations or anything else that
    // modifies eflags.
    // LAB 4: Your code here.
    addl $0x4, %esp
    popfl
    
	// Switch back to the adjusted trap-time stack.
    // LAB 4: Your code here.
    popl %esp

    // Return to re-execute the instruction that faulted.
    // LAB 4: Your code here.
    ret 
```

# Exercise 11

> Finish `set_pgfault_handler()` in `lib/pgfault.c`.

进程在运行前要注册自己的页面错误处理函数，这里是调用了之前我们实现的`sys_env_set_pgfault_upcall()`函数：

```javascript
void
set_pgfault_handler(void (*handler)(struct UTrapframe *utf))
{
	int r;

	if (_pgfault_handler == 0) {
		// First time through!
		// LAB 4: Your code here.
		if(sys_page_alloc(thisenv->env_id, (void *)(UXSTACKTOP - PGSIZE), PTE_W | PTE_U | PTE_P)){
			panic("set_pgfault_handler page_alloc failed");
		}
		if(sys_env_set_pgfault_upcall(thisenv->env_id, _pgfault_upcall)){
			panic("set_pgfault_handler set_pgfault_upcall failed");
		}
	}

	// Save handler pointer for assembly to call.
	_pgfault_handler = handler;
}
```

# Exercise 12

> Implement `fork`, `duppage` and `pgfault` in `lib/fork.c`.
> 
> Test your code with the forktree program. It should produce the following messages, with interspersed 'new env', 'free env', and 'exiting gracefully' messages. The messages may not appear in this order, and the environment IDs may be different.
> ```
>	1000: I am ''
>	1001: I am '0'
>	2000: I am '00'
>	2001: I am '000'
>	1002: I am '1'
>	3000: I am '11'
>	3001: I am '10'
>	4000: I am '100'
>	1003: I am '01'
>	5000: I am '010'
>	4001: I am '011'
>	2002: I am '110'
>	1004: I am '001'
>	1005: I am '111'
>	1006: I am '101'
>```

首先是`pgfault`处理页面错误时的写时拷贝：

```javascript
static void
pgfault(struct UTrapframe *utf)
{
    int r;
    void *addr = (void *) utf->utf_fault_va;
    uint32_t err = utf->utf_err;

    if ((err & FEC_WR) == 0 || (uvpt[PGNUM(addr)] & PTE_COW) == 0)
        panic("pgfault: it's not writable or attempt to access a non-cow page!");
    // Allocate a new page, map it at a temporary location (PFTEMP),
    // copy the data from the old page to the new page, then move the new
    // page to the old page's address.

    envid_t envid = sys_getenvid();
    if ((r = sys_page_alloc(envid, (void *)PFTEMP, PTE_P | PTE_W | PTE_U)) < 0)
        panic("pgfault: page allocation failed %e", r);

    addr = ROUNDDOWN(addr, PGSIZE);
    memmove(PFTEMP, addr, PGSIZE);
    if ((r = sys_page_unmap(envid, addr)) < 0)
        panic("pgfault: page unmap failed %e", r);
    if ((r = sys_page_map(envid, PFTEMP, envid, addr, PTE_P | PTE_W |PTE_U)) < 0)
        panic("pgfault: page map failed %e", r);
    if ((r = sys_page_unmap(envid, PFTEMP)) < 0)
        panic("pgfault: page unmap failed %e", r);
}
```

在`pgfault`函数中先判断是否页错误是由写时拷贝造成的，如果不是则panic。借用了一个一定不会被用到的位置PFTEMP，专门用来发生page fault的时候拷贝内容用的。先解除addr原先的页映射关系，然后将addr映射到PFTEMP映射的页，最后解除PFTEMP的页映射关系。

接下来是`duppage`函数，负责进行COW方式的页复制，将当前进程的第pn页对应的物理页的映射到envid的第pn页上去，同时将这一页都标记为COW。

```javascript
static int
duppage(envid_t envid, unsigned pn)
{
    int r;

    void *addr;
    pte_t pte;
    int perm;

    addr = (void *)((uint32_t)pn * PGSIZE);
    pte = uvpt[pn];
    perm = PTE_P | PTE_U;
    if ((pte & PTE_W) || (pte & PTE_COW))
        perm |= PTE_COW;
    if ((r = sys_page_map(thisenv->env_id, addr, envid, addr, perm)) < 0) {
        panic("duppage: page remapping failed %e", r);
        return r;
    }
    if (perm & PTE_COW) {
        if ((r = sys_page_map(thisenv->env_id, addr, thisenv->env_id, addr, perm)) < 0) {
            panic("duppage: page remapping failed %e", r);
            return r;
        }
    }
    return 0;
}
```

最后是`fork`函数，将页映射拷贝过去，这里需要考虑的地址范围就是从UTEXT到UXSTACKTOP为止，而在此之上的范围因为都是相同的，在`env_alloc`的时候已经设置好了。

```javascript
envid_t
fork(void)
{
	// LAB 4: Your code here.
	set_pgfault_handler(pgfault);

	envid_t envid = sys_exofork();
	uint8_t *addr;
	if (envid < 0)
		panic("sys_exofork:%e", envid);
	if (envid == 0) {
		thisenv = &envs[ENVX(sys_getenvid())];
		return 0;
	}

	for (addr = (uint8_t *)UTEXT; addr < (uint8_t *)USTACKTOP; addr += PGSIZE) {
		if ((uvpd[PDX(addr)] & PTE_P) && (uvpt[PGNUM(addr)] & PTE_P)
				&& (uvpt[PGNUM(addr)] & PTE_U)) {
			duppage(envid, PGNUM(addr));
		}
	}

	duppage(envid, PGNUM(ROUNDDOWN(&addr, PGSIZE)));

	int r;
	if ((r = sys_page_alloc(envid, (void *)(UXSTACKTOP - PGSIZE), PTE_P|PTE_U|PTE_W)))
		panic("sys_page_alloc:%e", r);

	extern void _pgfault_upcall();
	sys_env_set_pgfault_upcall(envid, _pgfault_upcall);

	if ((r = sys_env_set_status(envid, ENV_RUNNABLE)))
		panic("sys_env_set_status:%e", r);

	return envid;
}
```

写时复制的总流程已在理论部分分析过了。至此，Lab4的part B的写时复制就完成了。

# Exercise 13

> Modify `kern/trapentry.S` and `kern/trap.c` to initialize the appropriate entries in the IDT and provide handlers for IRQs 0 through 15. Then modify the code in `env_alloc()` in `kern/env.c` to ensure that user environments are always run with interrupts enabled.
> 
> Also uncomment the sti instruction in `sched_halt()` so that idle CPUs unmask interrupts.
> 
> The processor never pushes an error code when invoking a hardware interrupt handler. You might want to re-read section 9.2 of the 80386 Reference Manual, or section 5.8 of the IA-32 Intel Architecture Software Developer's Manual, Volume 3, at this time.
> 
> After doing this exercise, if you run your kernel with any test program that runs for a non-trivial length of time (e.g., spin), you should see the kernel print trap frames for hardware interrupts. While interrupts are now enabled in the processor, JOS isn't yet handling them, so you should see it misattribute each interrupt to the currently running user environment and destroy it. Eventually it should run out of environments to destroy and drop into the monitor.

注意这里必须要全部将istrap值设为0。因为JOS中的这个istrap设为1就会在开始处理中断时将FL_IF置为1，而设为0则保持FL_IF不变，设为0才能通过trap()中对FL_IF的检查。

```javascript

//kern/trap.c/trap_init()

	...
	void handler32();
	void handler33();
	void handler36();
	void handler39();
	void handler46();
	void handler51();
	// IRQ
	SETGATE(idt[IRQ_TIMER + IRQ_OFFSET], 0, GD_KT, handler32, 0);
	SETGATE(idt[IRQ_KBD + IRQ_OFFSET], 0, GD_KT, handler33, 0);
	SETGATE(idt[IRQ_SERIAL + IRQ_OFFSET], 0, GD_KT, handler36, 0);
	SETGATE(idt[IRQ_SPURIOUS + IRQ_OFFSET], 0, GD_KT, handler39, 0);
	SETGATE(idt[IRQ_IDE + IRQ_OFFSET], 0, GD_KT, handler46, 0);
	SETGATE(idt[IRQ_ERROR + IRQ_OFFSET], 0, GD_KT, handler51, 0);
	...

//trapentry.S

	...
	TRAPHANDLER_NOEC(handler32, IRQ_OFFSET + IRQ_TIMER)
	TRAPHANDLER_NOEC(handler33, IRQ_OFFSET + IRQ_KBD)
	TRAPHANDLER_NOEC(handler36, IRQ_OFFSET + IRQ_SERIAL)
	TRAPHANDLER_NOEC(handler39, IRQ_OFFSET + IRQ_SPURIOUS)
	TRAPHANDLER_NOEC(handler46, IRQ_OFFSET + IRQ_IDE)
	TRAPHANDLER_NOEC(handler51, IRQ_OFFSET + IRQ_ERROR)
	...

```

然后修改`kern/env.c/env_alloc()`里的代码，确保始终在启用中断的情况下运行用户环境。

```javascript

	···
	// Enable interrupts while in user mode.
	// LAB 4: Your code here.
	e->env_tf.tf_eflags |= FL_IF;
	···

```

# Exercise 14

> Modify the kernel's `trap_dispatch()` function so that it calls `sched_yield()` to find and run a different environment whenever a clock interrupt takes place.
> 
> You should now be able to get the `user/spin` test to work: the parent environment should fork off the child, `sys_yield()` to it a couple times but in each case regain control of the CPU after one time slice, and finally kill the child environment and terminate gracefully.

修改`trap_dispatch`函数，当发生时钟中断时调用`sched_yield`函数来调度下一个进程。

```javascript

	...
	// Handle clock interrupts. Don't forget to acknowledge the
	// interrupt using lapic_eoi() before calling the scheduler!
	// LAB 4: Your code here.
	if (tf->tf_trapno == IRQ_OFFSET + IRQ_TIMER) {
		lapic_eoi();
		sched_yield();
		return;
	}
	...

```

然而到这里之后再运行`make grade`，我一直无法通过`trap()`中的`assert(!(read_eflags() & FL_IF))`这条语句;后来发现是在设置页面错误中断门描述符的时候istrap参数设成了1，`SETGATE(idt[T_PGFLT], 1, GD_KT, pgflt_handler, 0)`；陷阱门是不会重置FL_IF位的，导致内核态下中断没关。

# Exercise 15

> Implement `sys_ipc_recv` and `sys_ipc_try_send` in `kern/syscall.c`. Read the comments on both before implementing them, since they have to work together. When you call `envid2env` in these routines, you should set the checkperm flag to 0, meaning that any environment is allowed to send IPC messages to any other environment, and the kernel does no special permission checking other than verifying that the target envid is valid.
>
> Then implement the `ipc_recv` and `ipc_send` functions in `lib/ipc.c`.
>
> Use the `user/pingpong` and `user/primes` functions to test your IPC mechanism. `user/primes` will generate for each prime number a new environment until JOS runs out of environments. You might find it interesting to read `user/primes.c` to see all the forking and IPC going on behind the scenes.

完成IPC功能，别忘记在syscall里面分发加新增的两个系统调用。需要注意的是因为`sys_ipc_recv`如果成功是没有返回值的，所以我们需要在`sys_ipc_try_send`中为其设置返回值。

```javascript
static int
sys_ipc_recv(void *dstva)
{
	// LAB 4: Your code here.

	if (dstva < (void *)UTOP && PGOFF(dstva)) return -E_INVAL;
	curenv->env_ipc_recving = 1;
	curenv->env_ipc_dstva = dstva;
	curenv->env_status = ENV_NOT_RUNNABLE;
	sys_yield();
	//panic("sys_ipc_recv not implemented");
	return 0;
}

static int
sys_ipc_try_send(envid_t envid, uint32_t value, void *srcva, unsigned perm)
{
	// LAB 4: Your code here.
	struct Env* e;
	int r;
	if ((r = envid2env(envid, &e, 0)) < 0)
		return -E_BAD_ENV;
	if (!e->env_ipc_recving)
		return -E_IPC_NOT_RECV;
	if (srcva < (void *)UTOP) {
		if (PGOFF(srcva)) return -E_INVAL;
		if ((perm & PTE_U) == 0 || (perm & PTE_P) == 0)
            return -E_INVAL;
    	if ((perm & ~(PTE_U | PTE_P | PTE_W | PTE_AVAIL)) != 0)
            return -E_INVAL;
		pte_t *pte;
		struct PageInfo *p = page_lookup(curenv->env_pgdir, srcva, &pte);
		if (!p) return -E_INVAL;
		if((perm & PTE_W) && !(*pte & PTE_W))
			return -E_INVAL;
		if (e->env_ipc_dstva < (void *)UTOP) {
			if ((r = page_insert(e->env_pgdir, p, e->env_ipc_dstva, perm)) < 0) 
				return -E_NO_MEM;
			e->env_ipc_perm = perm;
		}
	}
	e->env_ipc_recving = 0;
	e->env_ipc_from = curenv->env_id;
	e->env_ipc_value = value;
	e->env_status = ENV_RUNNABLE;
	e->env_tf.tf_regs.reg_eax=0;

	return 0;
}

void
ipc_send(envid_t to_env, uint32_t val, void *pg, int perm)
{
	// LAB 4: Your code here.
	if (!pg) pg = (void *)UTOP;
	int r;
	while ((r = sys_ipc_try_send(to_env, val, pg, perm))) {
		if (r != -E_IPC_NOT_RECV) panic("ipc_send error %e", r);
		sys_yield();
	}
}

int32_t
ipc_recv(envid_t *from_env_store, void *pg, int *perm_store)
{
	// LAB 4: Your code here.
	if (!pg || pg > (void *)UTOP) pg = (void *)UTOP;
	int t = sys_ipc_recv(pg);
	if (!t) {
		if (from_env_store) *from_env_store = thisenv->env_ipc_from;
		if (perm_store) *perm_store = thisenv->env_ipc_perm; 
	} else {
		if (from_env_store) *from_env_store = 0;
		if (perm_store) *perm_store = 0; 
		return t;
	}
	
	return thisenv->env_ipc_value;
}
```

至此，lab4的全部内容已经完成。