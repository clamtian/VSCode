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

> Boot your kernel, running `user/evilhello`. The environment should be destroyed, and the kernel should not panic. You should see:
> ```javascript
>	[00000000] new env 00001000
>	...
>	[00001000] user_mem_check assertion failure for va f010000c
>	[00001000] free env 00001000
> ```

按照要求运行一下`user/evilhello`，如果出现上述信息，证明你这个lab做的是成功的。运行`make grade`可以打分，看看自己有没有错误的地方。





