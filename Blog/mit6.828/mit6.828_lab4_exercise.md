# Exercise 1
> Implement `mmio_map_region` in `kern/pmap.c`. To see how this is used, look at the beginning of `lapic_init` in `kern/lapic.c`. You'll have to do the next exercise, too, before the tests for `mmio_map_region` will run.

```Javascript
void *
mmio_map_region(physaddr_t pa, size_t size)
{
	static uintptr_t base = MMIOBASE;

	physaddr_t pa_begin = ROUNDDOWN(pa, PGSIZE);
	physaddr_t pa_end = ROUNDUP(pa + size, PGSIZE);
	if (pa_end - pa_begin >= MMIOLIM - MMIOBASE) {
	    panic("mmio_map_region: requesting size too large.\n");
	}
	size = pa_end - pa_begin;
	boot_map_region(kern_pgdir, base, size, pa_begin, PTE_W | PTE_PCD | PTE_PWT);
	void *ret = (void *)base;
	base += size;
	return ret;
}
```

# Exercise 2
> Read `boot_aps()` and `mp_main()` in `kern/init.c`, and the assembly code in `kern/mpentry.S`. Make sure you understand the control flow transfer during the bootstrap of APs. Then modify your implementation of `page_init()` in `kern/pmap.c` to avoid adding the page at `MPENTRY_PADDR` to the free list, so that we can safely copy and run AP bootstrap code at that physical address. Your code should pass the updated `check_page_free_list()` test (but might fail the updated `check_kern_pgdir()` test, which we will fix soon).

修改 `kern/pmap.c`中的`page_init()`使其不要将 `MPENTRY_PADDR`(0x7000)这一页加入到`page_free_list`。

```javaScript
void
page_init(void)
{
	size_t i;
	page_free_list = NULL;
	pages[0].pp_ref = 1;
	pages[0].pp_link = NULL;
	size_t index_IOhole_begin = npages_basemem;
	size_t index_alloc_end = PADDR(boot_alloc(0)) / PGSIZE;

	for (i = 1; i < npages; i++) {
		if((i >= index_IOhole_begin && i < index_alloc_end) || (i == MPENTRY_PADDR / PGSIZE)){
			pages[i].pp_ref = 1;
			pages[i].pp_link = NULL;
		}else{
			pages[i].pp_ref = 0;
			pages[i].pp_link = page_free_list;
			page_free_list = &pages[i];
		}
	}
}
```

## Question

> Compare `kern/mpentry.S` side by side with `boot/boot.S`. Bearing in mind that `kern/mpentry.S` is compiled and linked to run above `KERNBASE` just like everything else in the kernel, what is the purpose of macro `MPBOOTPHYS`? Why is it necessary in `kern/mpentry.S` but not in `boot/boot.S`? In other words, what could go wrong if it were omitted in `kern/mpentry.S`?
>
> Hint: recall the differences between the link address and the load address that we have discussed in Lab 1.

 这是因为`mpentry.S`代码中`mpentry_start`和`mpentry_end`的地址都在`KERNBASE`(0xf0000000）之上，实模式无法寻址，而我们将`mpentry.S`加载到了0x7000处，所以需要通过`MPBOOTPHYS`来寻址。而`boot.S`加载的位置本身就是实模式可寻址的低地址，所以不用额外转换。

# Exercise 3

> Modify `mem_init_mp()` (in `kern/pmap.c`) to map per-CPU stacks starting at `KSTACKTOP`, as shown in `inc/memlayout.h`. The size of each stack is `KSTKSIZE` bytes plus `KSTKGAP` bytes of unmapped guard pages. Your code should pass the new check in `check_kern_pgdir()`.

修改 `mem_mp_init()`为每个cpu分配内核栈。注意，CPU内核栈之间有空白`KSTKGAP`(32KB)，其目的是为了避免一个CPU的内核栈覆盖另外一个CPU的内核栈，空出来这部分可以在栈溢出时报错。

```javascript
static void
mem_init_mp(void)
{
	int i;
	for(i = 0; i < NCPU; ++i){
		uintptr_t kstacktop_i = KSTACKTOP - i * (KSTKSIZE + KSTKGAP);
		boot_map_region(kern_pgdir, 
						kstacktop_i - KSTKSIZE, 
						KSTKSIZE, 
						(physaddr_t)PADDR(&percpu_kstacks[i]), 
						PTE_P | PTE_W);
	}
}
```

# Exercise 4

> The code in `trap_init_percpu()` (`kern/trap.c`) initializes the TSS and TSS descriptor for the BSP. It worked in Lab 3, but is incorrect when running on other CPUs. Change the code so that it can work on all CPUs. (Note: your new code should not use the global ts variable any more.)

修改`trap_init_percpu()`，完成每个CPU的TSS初始化。设置ts_esp0和ts_ss0，注意，设置全局描述符的时候加上cpu_id作为索引值，ltr时要注意是加载的描述符的偏移值，所以记得cpu_id<<3。

```javascript
void
trap_init_percpu(void)
{
	int id = thiscpu->cpu_id;
	thiscpu->cpu_ts.ts_esp0 = KSTACKTOP - (KSTKSIZE + KSTKGAP) * id;
	thiscpu->cpu_ts.ts_ss0 = GD_KD;
	thiscpu->cpu_ts.ts_iomb = sizeof(struct Taskstate);

	gdt[(GD_TSS0 >> 3) + id] = SEG16(STS_T32A, (uint32_t) (&thiscpu->cpu_ts),
					sizeof(struct Taskstate) - 1, 0);
	gdt[(GD_TSS0 >> 3) + id].sd_s = 0;
	ltr(GD_TSS0 + (id << 3));

	lidt(&idt_pd);
}
```

# Exercise 5

> Apply the big kernel lock as described above, by calling `lock_kernel()` and `unlock_kernel()` at the proper locations.

```javascript
// 加锁位置1 i386_init()的 boot_aps()函数前。
	...

    lock_kernel();
 
    // Starting non-boot CPUs
    boot_aps();

	...

// 加锁位置2 mp_main()的函数末尾，这里还要加上 sched_yield()。
	...

    lock_kernel();
	sched_yield();

	...

// 加锁位置3 trap()里面
	...

    lock_kernel();
    assert(curenv);
    
	...            
// 释放锁
	...

    lcr3(PADDR(curenv->env_pgdir));
    unlock_kernel();
    env_pop_tf(&curenv->env_tf);

	...
```

## Question

> It seems that using the big kernel lock guarantees that only one CPU can run the kernel code at a time. Why do we still need separate kernel stacks for each CPU? Describe a scenario in which using a shared kernel stack will go wrong, even with the protection of the big kernel lock.

这是因为虽然内核锁限制了多个进程同时执行内核代码，但是在陷入`trap()`之前，CPU硬件已经自动压栈了SS, ESP, EFLAGS, CS, EIP等寄存器内容，而且在`trapentry.S`中也压入了错误码和中断号到内核栈中，所以不同CPU必须分开内核栈，否则多个CPU同时陷入内核时会破坏栈结构，此时都还没有进入到`trap()`的加内核锁位置。

## Challenge

> The big kernel lock is simple and easy to use. Nevertheless, it eliminates all concurrency in kernel mode. Most modern operating systems use different locks to protect different parts of their shared state, an approach called fine-grained locking. Fine-grained locking can increase performance significantly, but is more difficult to implement and error-prone. If you are brave enough, drop the big kernel lock and embrace concurrency in JOS!
> 
> It is up to you to decide the locking granularity (the amount of data that a lock protects). As a hint, you may consider using spin locks to ensure exclusive access to these shared components in the JOS kernel:
>
> * The page allocator.
> * The console driver.
> * The scheduler.
> * The inter-process communication (IPC) state that you will implement in the part C.

留做

# Exercise 6

> Implement round-robin scheduling in `sched_yield()` as described above. Don't forget to modify `syscall()` to dispatch `sys_yield()`.
> 
> Make sure to invoke `sched_yield()` in mp_main.
> 
> Modify `kern/init.c` to create three (or more!) environments that all run the program `user/yield.c`.
> 
> Run make qemu. You should see the environments switch back and forth between each other five times before terminating, like below.
> 
> Test also with several CPUS: `make qemu CPUS=2`.
> 
> ```
> Hello, I am environment 00001000.
> Hello, I am environment 00001001.
> Hello, I am environment 00001002.
> Back in environment 00001000, iteration 0.
> Back in environment 00001001, iteration 0.
> Back in environment 00001002, iteration 0.
> Back in environment 00001000, iteration 1.
> Back in environment 00001001, iteration 1.
> Back in environment 00001002, iteration 1.
> ```
> After the yield programs exit, there will be no runnable environment in the system, the scheduler should invoke the JOS kernel monitor. If any of this does not happen, then fix your code before proceeding.

实现轮转调度。另外还要修改`kern/syscall.c`加入对 `SYS_yield` 的支持，并在`kern/init.c`中加载3个`user_yield`进程测试。

```javascript
void
sched_yield(void)
{
	struct Env *idle;
	idle = curenv;
	int idx = idle ? ENVX(idle->env_id) : -1;

	int i;
	for(i = idx + 1; i < NENV; i++){
		if(envs[i].env_status == ENV_RUNNABLE){
			env_run(&envs[i]);
		}
	}
	for(i = 0; i <= idx; i++){
		if(envs[i].env_status == ENV_RUNNABLE){
			env_run(&envs[i]);
		}
	}
	if(idle && idle->env_status == ENV_RUNNING) {
		env_run(idle);
	}

	sched_halt();
}

// kern/syscall.c修改
	...
	
	case SYS_yield:
        sys_yield();
        return 0;
   
    ...

// 修改测试进程     
	...

    // Touch all you want.
    ENV_CREATE(user_primes, ENV_TYPE_USER);
    // ENV_CREATE(user_primes, ENV_TYPE_USER);
    ENV_CREATE(user_yield, ENV_TYPE_USER);
    ENV_CREATE(user_yield, ENV_TYPE_USER);
    ENV_CREATE(user_yield, ENV_TYPE_USER);
	
	...
```

## Question
> In your implementation of `env_run()` you should have called `lcr3()`. Before and after the call to `lcr3()`, your code makes references (at least it should) to the variable e, the argument to `env_run`. Upon loading the %cr3 register, the addressing context used by the MMU is instantly changed. But a virtual address (namely e) has meaning relative to a given address context--the address context specifies the physical address to which the virtual address maps. Why can the pointer e be dereferenced both before and after the addressing switch?

这是因为所有的进程`env_pgdir`的高地址的映射跟`kern_pgdir`的是一样的，见实验3的`env_setup_vm()`。

> Whenever the kernel switches from one environment to another, it must ensure the old environment's registers are saved so they can be restored properly later. Why? Where does this happen?

当然要保存寄存器状态，以知道下一条指令地址以及进程栈的状态，不然我们不知道从哪里继续运行。保存寄存器状态的代码是 `trap.c` 中的 `curenv->env_tf = *tf`。

## Challenge

> Add a less trivial scheduling policy to the kernel, such as a fixed-priority scheduler that allows each environment to be assigned a priority and ensures that higher-priority environments are always chosen in preference to lower-priority environments. If you're feeling really adventurous, try implementing a Unix-style adjustable-priority scheduler or even a lottery or stride scheduler. (Look up "lottery scheduling" and "stride scheduling" in Google.)
> 
> Write a test program or two that verifies that your scheduling algorithm is working correctly (i.e., the right environments get run in the right order). It may be easier to write these test programs once you have implemented fork() and IPC in parts B and C of this lab.

留做

## Challenge

> The JOS kernel currently does not allow applications to use the x86 processor's x87 floating-point unit (FPU), MMX instructions, or Streaming SIMD Extensions (SSE). Extend the Env structure to provide a save area for the processor's floating point state, and extend the context switching code to save and restore this state properly when switching from one environment to another. The FXSAVE and FXRSTOR instructions may be useful, but note that these are not in the old i386 user's manual because they were introduced in more recent processors. Write a user-level test program that does something cool with floating-point.

留做

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

## Challenge

> Add the additional system calls necessary to read all of the vital state of an existing environment as well as set it up. Then implement a user mode program that forks off a child environment, runs it for a while (e.g., a few iterations of sys_yield()), then takes a complete snapshot or checkpoint of the child environment, runs the child for a while longer, and finally restores the child environment to the state it was in at the checkpoint and continues it from there. Thus, you are effectively "replaying" the execution of the child environment from an intermediate state. Make the child environment perform some interaction with the user using sys_cgetc() or readline() so that the user can view and mutate its internal state, and verify that with your checkpoint/restart you can give the child environment a case of selective amnesia, making it "forget" everything that happened beyond a certain point.

留做

完成后运行`make grade`，应该可以看到 Part A得分为5分。























# Exercise 8

> Add the required code to the user library, then boot your kernel. You should see `user/hello` print "`hello, world`" and then print "`i am environment 00001000`". `user/hello` then attempts to "exit" by calling `sys_env_destroy()` (see `lib/libmain.c` and `lib/exit.c`). Since the kernel currently only supports one user environment, it should report that it has destroyed the only environment and then drop into the kernel monitor. You should be able to get make grade to succeed on the hello test.

获得当前正在运行的用户环境的 `env_id` , 以及这个用户环境所对应的 Env 结构体的指针。 `env_id` 我们可以通过调用 `sys_getenvid()` 这个函数来获得。Env 结构体的指针我们可以通过`env_id`来获得，`env_id`的值包含三部分，第31位被固定为0；第10~30这21位是标识符，标示这个用户环境；第0~9位代表这个用户环境所采用的 Env 结构体，在envs数组中的索引。所以我们只需知道 `env_id` 的0~9 位，我们就可以获得这个用户环境对应的 Env 结构体了。

```javascript
void
libmain(int argc, char **argv)
{
	// set thisenv to point at our Env structure in envs[].
	// LAB 3: Your code here.
	thisenv = &envs[ENVX(sys_getenvid())];

	// save the name of the program so that panic() can use it
	if (argc > 0)
		binaryname = argv[0];

	// call user main routine
	umain(argc, argv);

	// exit gracefully
	exit();
}
```

# Exercise 9

> Change `kern/trap.c` to panic if a page fault happens in kernel mode.
>
> Hint: to determine whether a fault happened in user mode or in kernel mode, check the low bits of the tf_cs.
>
> Read `user_mem_assert` in `kern/pmap.c` and implement `user_mem_check` in that same file.
>
> Change `kern/syscall.c` to sanity check arguments to system calls.
>
> Boot your kernel, running `user/buggyhello`. The environment should be destroyed, and the kernel should not panic. You should see:
> ```javascript
>	[00001000] user_mem_check assertion failure for va 00000001
>	[00001000] free env 00001000
>	Destroyed the only environment - nothing more to do!
> ```	
> Finally, change `debuginfo_eip` in `kern/kdebug.c` to call `user_mem_check` on usd, stabs, and stabstr. If you now run `user/breakpoint`, you should be able to run backtrace from the kernel monitor and see the backtrace traverse into `lib/libmain.c` before the kernel panics with a page fault. What causes this page fault? You don't need to fix it, but you should understand why it happens.

当页面错误发生时，我们需要判断引起页面错误的代码是在内核中还是在用户程序中，根据 CS 段寄存器的低2位，这两位的名称叫做 CPL 位，表示当前运行的代码的访问权限级别，0代表是内核态，3代表是用户态。如果这个 `page fault` 是出现在内核中时，要把这个事件 `panic` 出来，所以我们把 `page_fault_handler` 文件修改如下：

```javascript
	...

	// Handle kernel-mode page faults.

	// LAB 3: Your code here.
	if(!(tf->tf_cs && 0x01)) panic("kernel-mode page fault, fault address %d\n", fault_va);

	...
```

接下来继续完善 `kern/pmap.c` 文件中的 `user_mem_check` 函数，通过观察 `user_mem_assert` 函数我们发现，它调用了 `user_mem_check` 函数。而 `user_mem_check` 函数的功能是检查一下当前用户态程序是否有对虚拟地址空间 [va, va+len] 的 `perm| PTE_P` 访问权限。搜易我们需要先找到这个虚拟地址范围对应于当前用户态程序的页表中的页表项，然后再去看一下这个页表项中有关访问权限的字段，是否包含 `perm | PTE_P`，只要有一个页表项是不包含的，就代表程序对这个范围的虚拟地址没有 `perm|PTE_P` 的访问权限。

```javascript
int
user_mem_check(struct Env *env, const void *va, size_t len, int perm)
{
	// LAB 3: Your code here.

	char *start, *end;
	start = ROUNDDOWN((char *)va, PGSIZE);
	end = ROUNDUP((char *)(va + len), PGSIZE);
	pte_t *cur = NULL;

	for(; start < end; start += PGSIZE){
		cur = pgdir_walk(env->env_pgdir, start, 0);
		if(!cur || (int)start >= ULIM || !(*cur & PTE_P) || ((int)*cur & perm) != perm){
			user_mem_check_addr = (void *)start < va ? (uintptr_t)va : (uintptr_t)start;
			return -E_FAULT;
		}
	}
	return 0;
}
```

我们还要补全 `kern/syscall.c` 文件中的 `sys_cputs` 函数，这个函数要求检查用户程序对虚拟地指空间 [s, s+len] 是否有访问权限:

```javascript
static void
sys_cputs(const char *s, size_t len)
{
	// Check that the user has permission to read memory [s, s+len).
	// Destroy the environment if not.
	user_mem_assert(curenv, s, len, 0);
	// LAB 3: Your code here.
	// Print the string supplied by the user.
	cprintf("%.*s", len, s);
}
```

最后在`kern/kdebug`中修改`debuginfo_eip`函数，对用户空间的数据使用`user_mem_check()`函数检查当前用户空间是否对其有`PTE_U`权限。

```javascript
		...

		// Make sure this memory is valid.
		// Return -1 if it is not.  Hint: Call user_mem_check.
		// LAB 3: Your code here.
		if(user_mem_check(curenv, usd, sizeof(*usd), PTE_U) < 0) return -1;
		
		stabs = usd->stabs;
		stab_end = usd->stab_end;
		stabstr = usd->stabstr;
		stabstr_end = usd->stabstr_end;

		// Make sure the STABS and string table memory is valid.
		// LAB 3: Your code here.
		if(user_mem_check(curenv, stabs, sizeof(*stabs), PTE_U) < 0 || user_mem_check(curenv, stabstr, sizeof(*stabstr), PTE_U) < 0)
			return -1;
		
		...
```

# Exercise 10

> Boot your kernel, running `user/evilhello`. The environment should be destroyed, and the kernel should not panic. You should see:
> ```javascript
>	[00000000] new env 00001000
>	...
>	[00001000] user_mem_check assertion failure for va f010000c
>	[00001000] free env 00001000
> ```

按照要求运行一下`user/evilhello`，如果出现上述信息，证明你这个lab做的是成功的。运行`make grade`可以打分，看看自己有没有错误的地方。





