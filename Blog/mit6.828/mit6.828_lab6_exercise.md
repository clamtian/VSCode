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

>  `spawn` relies on the new syscall `sys_env_set_trapframe` to initialize the state of the newly created environment. Implement `sys_env_set_trapframe` in `kern/syscall.c` (don't forget to dispatch the new system call in `syscall()`).
>
> Test your code by running the `user/spawnhello` program from `kern/init.c`, which will attempt to spawn `/hello` from the file system.
>
> Use make grade to test your code.

别忘了在`syscall()`中添加相应项。

```javascript
static int
sys_env_set_trapframe(envid_t envid, struct Trapframe *tf)
{
	// LAB 5: Your code here.
	// Remember to check whether the user has supplied us with a good
	// address!
    struct Env *e;
    int r;

    if ((r = envid2env(envid, &e, 1)) < 0)
        return -E_BAD_ENV;
	user_mem_assert(e, (const void *) tf, sizeof(struct Trapframe), PTE_U);
    e->env_tf = *tf;
    e->env_tf.tf_eflags |= FL_IF;
    e->env_tf.tf_eflags &= ~FL_IOPL_MASK;
	return 0; 
}
```

# Exercise 8

> Change duppage in `lib/fork.c` to follow the new convention. If the page table entry has the PTE_SHARE bit set, just copy the mapping directly. (You should use PTE_SYSCALL, not 0xfff, to mask out the relevant bits from the page table entry. 0xfff picks up the accessed and dirty bits as well.)
>
> Likewise, implement `copy_shared_pages` in `lib/spawn.c`. It should loop through all page table entries in the current process (just like fork did), copying any page mappings that have the PTE_SHARE bit set into the child process.

修改 `lib/fork.c` 和 `lib/spawn.c`，支持共享页面。

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
    if (pte & PTE_SHARE) {
        if ((r = sys_page_map(thisenv->env_id, addr, envid, addr, PTE_SYSCALL)) < 0) 
            panic("duppage: page remapping failed %e", r);
    } else if (pte & (PTE_W | PTE_COW)) {
        perm |= PTE_COW;
        if ((r = sys_page_map(thisenv->env_id, addr, envid, addr, perm)) < 0) 
            panic("duppage: page remapping failed %e", r);
        if ((r = sys_page_map(thisenv->env_id, addr, thisenv->env_id, addr, perm)) < 0) 
            panic("duppage: page remapping failed %e", r);
    } else {
		if ((r = sys_page_map(thisenv->env_id, addr, envid, addr, PTE_U|PTE_P)) < 0)
			panic("duppage: page remapping failed %e", r);
	}
    return 0;
}

static int
copy_shared_pages(envid_t child)
{
	// LAB 5: Your code here.
	
    uintptr_t addr;
    for (addr = 0; addr < UTOP; addr += PGSIZE) {
        if ((uvpd[PDX(addr)] & PTE_P) && (uvpt[PGNUM(addr)] & PTE_P) &&
                (uvpt[PGNUM(addr)] & PTE_U) && (uvpt[PGNUM(addr)] & PTE_SHARE)) {
            sys_page_map(thisenv->env_id, (void*)addr, child, (void*)addr, PTE_SYSCALL);
        }
    }
    return 0;
}
```

# Exercise 9

>  In your `kern/trap.c`, call `kbd_intr` to handle trap IRQ_OFFSET+IRQ_KBD and `serial_intr` to handle trap IRQ_OFFSET+IRQ_SERIAL.

用户键盘输入会产生键盘中断IRQ_KBD(通过QEMU图形界面输入触发)或者串口中断IRQ_SERIAL(通过console触发)，为此要在`trap.c`中处理这两个中断，分别调用 `kbd_intr()` 和 `serial_intr()` 即可。


```javascript
	// Handle keyboard and serial interrupts.
	// LAB 5: Your code here.
	if (tf->tf_trapno == IRQ_OFFSET + IRQ_KBD){
		kbd_intr();
		return;
	} 
	else if (tf->tf_trapno == IRQ_OFFSET + IRQ_SERIAL){
		serial_intr();
		return;
	}
```

# Exercise 10

> The shell doesn't support I/O redirection. It would be nice to run `sh <script` instead of having to type in all the commands in the script by hand, as you did above. Add I/O redirection for < to `user/sh.c`.
>
> Test your implementation by typing `sh <script` into your shell 
>
> Run make `run-testshell` to test your shell. testshell simply feeds the above commands (also found in `fs/testshell.sh`) into the shell and then checks that the output matches `fs/testshell.key`.

```javascript
	case '<':	// Input redirection
		// Grab the filename from the argument list
		if (gettoken(0, &t) != 'w') {
			cprintf("syntax error: < not followed by word\n");
			exit();
		}
		// LAB 5: Your code here.
		if ((fd = open(t, O_RDONLY)) < 0) {
			cprintf("open %s for read: %e", t, fd);
			exit();
		}
		if (fd != 0) {
			dup(fd, 0); //应该是让文件描述符0也作为fd对应的那个open file的struct Fd页面
			close(fd);
		}
		//panic("< redirection not implemented");
		break;

```

至此，lab5的全部内容已经完成。