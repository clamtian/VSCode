# Exercise 1
> Modify `mem_init()` in `kern/pmap.c` to allocate and map the envs array. This array consists of exactly NENV instances of the Env structure allocated much like how you allocated the pages array. Also like the pages array, the memory backing envs should also be mapped user read-only at UENVS (defined in `inc/memlayout.h`) so user processes can read from this array.

> You should run your code and make sure `check_kern_pgdir()` succeeds.

这个exercise要求我们给`NENV`个Env结构体在内存中分配空间，并将 envs 结构体的物理地址映射到 从 `UENV` 所指向的线性地址空间，该线性地址空间允许用户访问且只读，所以页面权限被标记为`PTE_U`。

```Javascript
void
mem_init(void)
{
	...

	pages = (struct PageInfo *)boot_alloc(npages * sizeof(struct PageInfo));
	memset(pages, 0, npages * sizeof(struct PageInfo));
	
	/***********************************************************************************/
	envs = (struct Env *)boot_alloc(NENV * sizeof(struct Env));
	memset(envs, 0, NENV * sizeof(struct Env));
	/***********************************************************************************/

	...

	boot_map_region(kern_pgdir, UPAGES, PTSIZE, PADDR(pages), PTE_U);

	/***********************************************************************************/
	boot_map_region(kern_pgdir, UENVS, PTSIZE, PADDR(envs), PTE_U);
	/***********************************************************************************/
	
	...
}
```

# Exercise 2
> In the file env.c, finish coding the following functions:
>
> `env_init()`
> 
> Initialize all of the Env structures in the envs array and add them to the env_free_list. Also calls env_init_percpu, which configures the segmentation hardware with separate segments for privilege level 0 (kernel) and privilege level 3 (user).
> 
> `env_setup_vm()`
>
>Allocate a page directory for a new environment and initialize the kernel portion of the new environment's address space.
>
> `region_alloc()`
> 
> Allocates and maps physical memory for an environment
> 
> `load_icode()`
> 
> You will need to parse an ELF binary image, much like the boot loader already does, and load its contents into the user address space of a new environment.
> 
> `env_create()`
> 
>Allocate an environment with env_alloc and call load_icode to load an ELF binary into it.
>
> `env_run()`
> 
> Start a given environment running in user mode.
> 
> As you write these functions, you might find the new cprintf verb %e useful -- it prints a description corresponding to an error code. For example,
> ```JAvaScript
> r = -E_NO_MEM;
> panic("env_alloc: %e", r);
> ```
> 	
> will panic with the message "env_alloc: out of memory".

这个exercise要求我们实现几个与用户进程运行相关的函数。

## `env_init()`

`env_init()`函数很简单，就是遍历 `envs` 数组中的所有 Env 结构体，把每一个结构体的 `env_id` 字段置0，因为要求所有的 Env 在 `env_free_list` 中的顺序，要和它在 `envs` 中的顺序一致，所以需要采用头插法。　

```JavaScript
void
env_init(void)
{
	env_free_list = NULL;
	for(int i = NENV - 1; i >= 0; --i){
		envs[i].env_id = 0;
		envs[i].env_status = ENV_FREE;
		envs[i].env_link = env_free_list;

		env_free_list = &envs[i];
	}
	// Per-CPU part of the initialization
	env_init_percpu();
}
```

## `env_setup_vm()`

`env_setup_vm()` 函数主要是初始化新的用户环境的页目录表，不过只设置页目录表中和操作系统内核跟内核相关的页目录项，用户环境的页目录项不要设置，因为所有用户环境的页目录表中和操作系统相关的页目录项都是一样的（除了虚拟地址UVPT，这个也会单独进行设置），所以我们可以参照 `kern_pgdir` 中的内容来设置 `env_pgdir` 中的内容。

```JavaScript
static int
env_setup_vm(struct Env *e)
{
	int i;
	struct PageInfo *p = NULL;

	// Allocate a page for the page directory
	if (!(p = page_alloc(ALLOC_ZERO)))
		return -E_NO_MEM;

	// LAB 3: Your code here.
	e->env_pgdir = (pde_t *) page2kva(p);
	p->pp_ref++;
	memcpy(e->env_pgdir, kern_pgdir, PGSIZE);

	for (i = 0; i < PDX(UTOP); ++i) {
        e->env_pgdir[i] |= PTE_W | PTE_U;
    }

	// UVPT maps the env's own page table read-only.
	// Permissions: kernel R, user R
	e->env_pgdir[PDX(UVPT)] = PADDR(e->env_pgdir) | PTE_P | PTE_U;
	return 0;
}

```

## `region_alloc()`

`region_alloc()` 为用户环境分配物理空间，这里注意我们要先把起始地址和终止地址进行页对齐，对其之后我们就可以以页为单位，为其一个页一个页的分配内存，并且修改页目录表和页表。

```JavaScript
static void
region_alloc(struct Env *e, void *va, size_t len)
{
	void* start = (void *)ROUNDDOWN((uint32_t)va, PGSIZE);
    void* end = (void *)ROUNDUP((uint32_t)va+len, PGSIZE);
    struct PageInfo *p = NULL;
    void* i;
    int r;

    for(i = start; i < end; i += PGSIZE){
        p = page_alloc(0);
        if(p == NULL)
           panic(" region alloc failed: allocation failed.\n");

        r = page_insert(e->env_pgdir, p, i, PTE_W | PTE_U);
        if(r != 0)
            panic("region alloc failed.\n");
    }
}
```

## `load_icode()`

`load_icode()` 功能是为每一个用户进程设置它的初始代码区，堆栈以及处理器标识位。每个用户程序都是ELF文件，所以我们要解析该ELF文件。

```JavaScript
static void
load_icode(struct Env *e, uint8_t *binary)
{
	struct Proghdr *ph, *eph;
	struct Elf *elf = (struct Elf*)binary;
	
	if(elf->e_magic != ELF_MAGIC) 
		panic("There is something wrong in load _icode function!\n");
	
	ph = (struct Proghdr *) ((uint8_t *) elf + elf->e_phoff);
	eph = ph + elf->e_phnum;

	lcr3(PADDR(e->env_pgdir));   //load user pgdir
	
	for(; ph < eph; ph++) {
		if(ph->p_type == ELF_PROG_LOAD){
			region_alloc(e, (void *)ph->p_va, ph->p_memsz);
			memmove((void *)ph->p_va, (void *)(binary + ph->p_offset), ph->p_filesz);
			memset((void *)(ph->p_va + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);
		}
	}
	e->env_tf.tf_eip = elf->e_entry;
	lcr3(PADDR(kern_pgdir));  

	region_alloc(e, (void *)(USTACKTOP - PGSIZE), PGSIZE);
}
```

## `env_create()`

`env_create()` 是利用`env_alloc()`函数和`load_icode()`函数，加载一个ELF文件到用户环境中

```JavaScript
void
env_create(uint8_t *binary, enum EnvType type)
{
	struct Env *e;
	int res = env_alloc(&e, 0);
	if(res < 0) panic("load 1st env failed!\n");
	e->env_type = type;
	load_icode(e, binary);
}
```

## `env_run()`

`env_run()` 是真正开始运行一个用户环境，注意在运行用户进程之前需要将页目录表切换成用户的。

```JavaScript
void
env_run(struct Env *e)
{
	if(curenv && curenv->env_status == ENV_RUNNABLE){
		curenv->env_status = ENV_RUNNABLE;
	}
	curenv = e;
	curenv->env_status = ENV_RUNNING;
	++curenv->env_runs;

	lcr3(PADDR(curenv->env_pgdir));

	env_pop_tf(&(curenv->env_tf));
}
```

# Exercise 3

> Read Chapter 9, Exceptions and Interrupts in the 80386 Programmer's Manual (or Chapter 5 of the IA-32 Developer's Manual), if you haven't already.

这个exercise是让我们阅读一些关于中断和异常的资料，建议可以看一看，如果阅读英文比较吃力的话，可以去找一些博客来看。

# Exercise 4

> Edit `trapentry.S` and `trap.c` and implement the features described above. The macros `TRAPHANDLER` and `TRAPHANDLER_NOEC` in `trapentry.S` should help you, as well as the `T_*` defines in `inc/trap.h`. You will need to add an entry point in `trapentry.S` (using those macros) for each trap defined in `inc/trap.h`, and you'll have to provide `_alltraps` which the `TRAPHANDLER` macros refer to. You will also need to modify `trap_init()` to initialize the idt to point to each of these entry points defined in `trapentry.S`; the SETGATE macro will be helpful here.
>
> Your `_alltraps` should:
>
> * push values to make the stack look like a struct Trapframe
> * load GD_KD into %ds and %es
> * pushl %esp to pass a pointer to the Trapframe as an argument to trap()
> * call trap (can trap ever return?)
> * Consider using the pushal instruction; it fits nicely with the layout of the struct Trapframe.
>
> Test your trap handling code using some of the test programs in the user directory that cause exceptions before making any system calls, such as user/divzero. You should be able to get make grade to succeed on the divzero, softint, and badsegment tests at this point.

首先看一下 `trapentry.S` 文件，里面定义了两个宏定义，`TRAPHANDLER`和`TRAPHANDLER_NOEC`。他们的功能从汇编代码中可以看出：声明了一个全局符号`name`，并且这个符号是函数类型的，代表它是一个中断处理函数名。其实这里就是两个宏定义的函数。这两个函数就是当系统检测到一个中断/异常时，需要首先完成的一部分操作，包括：中断异常码，中断错误码(error code)。正是因为有些中断有中断错误码，有些没有，所以我们采用了两个宏定义函数。

```javascript
#define TRAPHANDLER(name, num)						\
	.globl name;		/* define global symbol for 'name' */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	pushl $(num);							\
	jmp _alltraps

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num)					\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps

.text

/*
 * Lab 3: Your code here for generating entry points for the different traps.
 */
 
TRAPHANDLER_NOEC(handler0, T_DIVIDE)
TRAPHANDLER_NOEC(handler1, T_DEBUG)
TRAPHANDLER_NOEC(handler2, T_NMI)
TRAPHANDLER_NOEC(handler3, T_BRKPT)
TRAPHANDLER_NOEC(handler4, T_OFLOW)
TRAPHANDLER_NOEC(handler5, T_BOUND)
TRAPHANDLER_NOEC(handler6, T_ILLOP)
TRAPHANDLER(handler7, T_DEVICE)
TRAPHANDLER_NOEC(handler8, T_DBLFLT)
TRAPHANDLER(handler10, T_TSS)
TRAPHANDLER(handler11, T_SEGNP)
TRAPHANDLER(handler12, T_STACK)
TRAPHANDLER(handler13, T_GPFLT)
TRAPHANDLER(handler14, T_PGFLT)
TRAPHANDLER_NOEC(handler16, T_FPERR)
TRAPHANDLER_NOEC(handler48, T_SYSCALL)
```

然后就会调用 `_alltraps`，`_alltraps`函数其实就是为了能够让程序在之后调用`trap.c`中的`trap`函数时，能够正确的访问到输入的参数，即`Trapframe`指针类型的输入参数`tf`。

```javascript
/*
 * Lab 3: Your code here for _alltraps
 */
_alltraps:
	pushl %ds
	pushl %es
	pushal
	movw $GD_KD, %ax
	movw %ax, %ds
	movw %ax, %es
	pushl %esp
	call trap /*never return*/

1:jmp 1b
```

最后在`trap.c`中实现`trap_init`函数，即在idt表中插入中断向量描述符，可以使用`SETGATE`宏实现：

`SETGATE`宏的定义：

* `#define SETGATE(gate, istrap, sel, off, dpl)`

其中`gate`是idt表的index入口，`istrap`判断是异常还是中断，`sel`为代码段选择符，`off`表示对应的处理函数地址，`dpl`表示触发该异常或中断的用户权限。

```javascript
void
trap_init(void)
{
	extern struct Segdesc gdt[];

		// LAB 3: Your code here.
	void handler0();
	void handler1();
	void handler2();
	void handler3();
	void handler4();
	void handler5();
	void handler6();
	void handler7();
	void handler8();
	void handler10();
	void handler11();
	void handler12();
	void handler13();
	void handler14();
	void handler15();
	void handler16();
	void handler48();

	SETGATE(idt[T_DIVIDE], 1, GD_KT, handler0, 0);
	SETGATE(idt[T_DEBUG], 1, GD_KT, handler1, 0);
	SETGATE(idt[T_NMI], 0, GD_KT, handler2, 0);

	// T_BRKPT DPL 3
	SETGATE(idt[T_BRKPT], 1, GD_KT, handler3, 3);

	SETGATE(idt[T_OFLOW], 1, GD_KT, handler4, 0);
	SETGATE(idt[T_BOUND], 1, GD_KT, handler5, 0);
	SETGATE(idt[T_ILLOP], 1, GD_KT, handler6, 0);
	SETGATE(idt[T_DEVICE], 1, GD_KT, handler7, 0);
	SETGATE(idt[T_DBLFLT], 1, GD_KT, handler8, 0);
	SETGATE(idt[T_TSS], 1, GD_KT, handler10, 0);
	SETGATE(idt[T_SEGNP], 1, GD_KT, handler11, 0);
	SETGATE(idt[T_STACK], 1, GD_KT, handler12, 0);
	SETGATE(idt[T_GPFLT], 1, GD_KT, handler13, 0);
	SETGATE(idt[T_PGFLT], 1, GD_KT, handler14, 0);
	SETGATE(idt[T_FPERR], 1, GD_KT, handler16, 0);

	// T_SYSCALL DPL 3
	SETGATE(idt[T_SYSCALL], 0, GD_KT, handler48, 3);
	// Per-CPU setup 
	trap_init_percpu();
}
```

## Challenge
> Challenge! You probably have a lot of very similar code right now, between the lists of `TRAPHANDLER` in `trapentry.S` and their installations in `trap.c`. Clean this up. Change the macros in `trapentry.S` to automatically generate a table for `trap.c` to use. Note that you can switch between laying down code and data in the assembler by using the directives `.text` and `.data`.

这块是让我们对`trapentry.S`中的代码进行重构，提高代码利用率。

宏定义修改：

```javascript
* Use ec = 1 for traps where the CPU automatically push an error code and ec = 0 for not.
 * Use user = 1 for a syscall; user = 0 for a normal trap.
 */
#define TRAPHANDLER(name, num, ec, user)      \
.text;                                        \
        .globl name;            /* define global symbol for 'name' */   \
        .type name, @function;  /* symbol type is function */           \
        .align 2;               /* align function definition */         \
        name:                   /* function starts here */              \
        .if ec==0;                                                      \
                pushl $0;                                               \
        .endif;                                                         \
        pushl $(num);                                                   \
        jmp _alltraps;                                                  \
.data;                                                                  \
        .long  name, num, user
```

修改`TRAPHANDLER`宏定义整合代码，在定义函数的同时(`.text`)定义相应的数据(`.data`)，然后定义1个全局数组，利用前面定义的宏实现该数组的填充。

```javascript
data
        .globl  entry_data
        entry_data:
.text
TRAPHANDLER(divide_entry, T_DIVIDE, 0, 0);
TRAPHANDLER(debug_entry, T_DEBUG, 0, 0);
TRAPHANDLER(nmi_entry, T_NMI, 0, 0);
TRAPHANDLER(brkpt_entry, T_BRKPT, 0, 1);
TRAPHANDLER(oflow_entry, T_OFLOW, 0, 0);
TRAPHANDLER(bound_entry, T_BOUND, 0, 0);
TRAPHANDLER(illop_entry, T_ILLOP, 0, 0);
TRAPHANDLER(device_entry, T_DEVICE, 0, 0);      
TRAPHANDLER(dblflt_entry, T_DBLFLT, 1, 0);
TRAPHANDLER(tts_entry, T_TSS, 1, 0);
TRAPHANDLER(segnp_entry, T_SEGNP, 1, 0);
TRAPHANDLER(stack_entry, T_STACK, 1, 0);
TRAPHANDLER(gpflt_entry, T_GPFLT, 1, 0);
TRAPHANDLER(pgflt_entry, T_PGFLT, 1, 0);
TRAPHANDLER(fperr_entry, T_FPERR, 0, 0);
TRAPHANDLER(align_entry, T_ALIGN, 1, 0);
TRAPHANDLER(mchk_entry, T_MCHK, 0, 0);
TRAPHANDLER(simderr_entry, T_SIMDERR, 0, 0);
TRAPHANDLER(syscall_entry, T_SYSCALL, 0, 1);
.data
        .long 0, 0, 0   // interupt end identify
```

最后在`trap_init`函数中就可以使用该全局数组对idt进行初始化。

```javascript
void
trap_init(void)
{
        extern struct Segdesc gdt[];
        extern long entry_data[][3];
        int i;

        for (i = 0; entry_data[i][0] != 0; i++ )
                SETGATE(idt[entry_data[i][1]], 0, GD_KT, entry_data[i][0], entry_data[i][2]*3);

        trap_init_percpu();
}
```

## Questions

> Answer the following questions in your answers-lab3.txt:
> 
> What is the purpose of having an individual handler function for each exception/interrupt? (i.e., if all exceptions/interrupts were delivered to the same handler, what feature that exists in the current implementation could not be provided?)

不同的中断或者异常需要不同的中断处理函数，因为不同的异常/中断需要不同的处理方式，比如有些异常是代表指令有错误，那么不会返回被中断的命令。而有些中断可能只是为了处理外部IO事件，此时执行完中断函数还要返回到被中断的程序中继续运行。

> Did you have to do anything to make the user/softint program behave correctly? The grade script expects it to produce a general protection fault (trap 13), but softint's code says int $14. Why should this produce interrupt vector 13? What happens if the kernel actually allows softint's int $14 instruction to invoke the kernel's page fault handler (which is interrupt vector 14)?

 因为当前的系统正在运行在用户态下，特权级为3，而INT指令为系统指令，特权级为0。特权级为3的程序不能直接调用特权级为0的程序，会引发一个`General Protection Exception`，即trap 13。

# Exercise 5

> Modify trap_dispatch() to dispatch page fault exceptions to page_fault_handler(). You should now be able to get make grade to succeed on the faultread, faultreadkernel, faultwrite, and faultwritekernel tests. If any of them don't work, figure out why and fix them. Remember that you can boot JOS into a particular user program using make run-x or make run-x-nox. For instance, make run-hello-nox runs the hello user program.

`Trapframe` 中的 `tf_trapno` 成员代表这个中断的中断向量，所以在 `trap_dispatch` 函数中我们需要根据输入的 `Trapframe` 指针中的 `tf_trapno` 成员来判断到来的中断是否是缺页中断，如果是则执行 `page_fault_handler` 函数。

```javascript
static void
trap_dispatch(struct Trapframe *tf)
{
	// Handle processor exceptions.
	// LAB 3: Your code here.
	if (tf->tf_trapno == T_PGFLT) {
		return page_fault_handler(tf);
	}
	// Unexpected trap: The user process or the kernel has a bug.
	print_trapframe(tf);
	if (tf->tf_cs == GD_KT)
		panic("unhandled trap in kernel");
	else {
		env_destroy(curenv);
		return;
	}
}
```

修改完上面的代码后，你的lab应该可以通过`faultread`, `faultreadkernel`, `faultwrite`, 和 `faultwritekernel`四个test，如果没有通过的话，需要查看是不是之前的代码有错误。

# Exercise 6

> Modify `trap_dispatch()` to make breakpoint exceptions invoke the kernel monitor. You should now be able to get make grade to succeed on the breakpoint test.

与上一个练习相似，这里我们需要把`breakpoint`异常引入到`monitor`函数中。

```javascript
static void
trap_dispatch(struct Trapframe *tf)
{
	// Handle processor exceptions.
	// LAB 3: Your code here.
	if (tf->tf_trapno == T_PGFLT) {
		return page_fault_handler(tf);
	}

	if (tf->tf_trapno == T_BRKPT) {
		return monitor(tf);
	}
	// Unexpected trap: The user process or the kernel has a bug.
	print_trapframe(tf);
	if (tf->tf_cs == GD_KT)
		panic("unhandled trap in kernel");
	else {
		env_destroy(curenv);
		return;
	}
	
}
```

## Challenge
> Modify the JOS kernel monitor so that you can 'continue' execution from the current location (e.g., after the int3, if the kernel monitor was invoked via the breakpoint exception), and so that you can single-step one instruction at a time. You will need to understand certain bits of the EFLAGS register in order to implement single-stepping.
> 
> Optional: If you're feeling really adventurous, find some x86 disassembler source code - e.g., by ripping it out of QEMU, or out of GNU binutils, or just write it yourself - and extend the JOS kernel monitor to be able to disassemble and display instructions as you are stepping through them. Combined with the symbol table loading from lab 1, this is the stuff of which real kernel debuggers are made.

留做

## Questions

> The break point test case will either generate a break point exception or a general protection fault depending on how you initialized the break point entry in the IDT (i.e., your call to `SETGATE` from `trap_init`). Why? How do you need to set it up in order to get the breakpoint exception to work as specified above and what incorrect setup would cause it to trigger a general protection fault?

在设置IDT表中的`breakpoint exception`的表项时，如果我们把表项中的DPL字段设置为3，则会触发`breakpoint exception`，如果设置为0，则会触发`general protection exception`。DPL字段代表的含义是段描述符优先级（Descriptor Privileged Level DPL），如果我们想要当前执行的程序能够跳转到这个描述符所指向的程序那里继续执行的话，有个要求，就是要求当前运行程序的CPL，RPL的最大值需要小于等于异常处理函数的DPL，否则就会出现优先级低的代码试图去访问优先级高的代码的情况，就会触发`general protection exception`。那么我们的测试程序首先运行于用户态，它的CPL为3，当异常发生时，它希望去执行 int 3指令，这是一个系统级别的指令，用户态命令的CPL大于 int3 的DPL，所以就会触发`general protection exception`，但是如果把IDT这个表项的DPL设置为3时，就不会出现这样的现象了。

> What do you think is the point of these mechanisms, particularly in light of what the `user/softint` test program does?

`user/softint`的内容：

```javascript
// buggy program - causes an illegal software interrupt

#include <inc/lib.h>

void
umain(int argc, char **argv)
{
	asm volatile("int $14");	// page fault
}
```

这个程序的本意是想触发一个`page fault`，但是在实际运行时触发的却是`General Protection fault`，这是因为在设置IDT表中的`page fault`的表项时，如果我们把表项中的DPL字段设置为了0。所以重点应该是要把异常/中断的特权级别分清楚。

# Exercise 7

> Add a handler in the kernel for interrupt vector `T_SYSCALL`. You will have to edit `kern/trapentry.S` and `kern/trap.c`'s `trap_init()`. You also need to change `trap_dispatch()` to handle the system call interrupt by calling `syscall()` (defined in `kern/syscall.c`) with the appropriate arguments, and then arranging for the return value to be passed back to the user process in %eax. Finally, you need to implement `syscall()` in `kern/syscall.c`. Make sure `syscall()` returns `-E_INVAL` if the system call number is invalid. You should read and understand `lib/syscall.c` (especially the inline assembly routine) in order to confirm your understanding of the system call interface. Handle all the system calls listed in `inc/syscall.h` by invoking the corresponding kernel function for each call.
>
> Run the `user/hello` program under your kernel (make `run-hello`). It should print "`hello, world`" on the console and then cause a page fault in user mode. If this does not happen, it probably means your system call handler isn't quite right. You should also now be able to get make grade to succeed on the testbss test.

关于系统调用的流程我们已经在理论篇分析过了，这个exercise让我们实现`T_SYSCALL`这个中断，首先就要在 `kern/trapentry.S` 文件中为它声明它的中断处理函数，即`TRAPHANDLER_NOEC`，然后需要在`trap_init()` 函数中为它注册，这些我们在之前的exercise中已经做了。接下来在函数`trap_dispatch()`中对系统调用分发到`syscall()`函数，这里我们首先对`trap_dispatch()`函数进行修改：

```javascript
	...
	if (tf->tf_trapno == T_SYSCALL) {
		tf->tf_regs.reg_eax = syscall(
			tf->tf_regs.reg_eax,
			tf->tf_regs.reg_edx,
			tf->tf_regs.reg_ecx,
			tf->tf_regs.reg_ebx,
			tf->tf_regs.reg_edi,
			tf->tf_regs.reg_esi
		);
		return;
	}
	...
```

然后在`syscall()`函数中根据不同的系统调用序号，执行不同的函数：

```javascript
// Dispatches to the correct kernel function, passing the arguments.
int32_t
syscall(uint32_t syscallno, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5)
{
	// Call the function corresponding to the 'syscallno' parameter.
	// Return any appropriate return value.
	// LAB 3: Your code here.
	switch (syscallno) {
		case SYS_cputs : 
			sys_cputs((const char*)a1, a2);
			return 0;
		case SYS_cgetc : 
			return sys_cgetc();
		case SYS_getenvid : 
			return sys_getenvid();
		case SYS_env_destroy : 
			return sys_env_destroy(a1);
		default:
			return -E_INVAL;
	}
}
```

## Challenge

> Challenge! Implement system calls using the sysenter and sysexit instructions instead of using int 0x30 and iret.
>
> The sysenter/sysexit instructions were designed by Intel to be faster than int/iret. They do this by using registers instead of the stack and by making assumptions about how the segmentation registers are used. The exact details of these instructions can be found in Volume 2B of the Intel reference manuals.
> 
> The easiest way to add support for these instructions in JOS is to add a sysenter_handler in kern/trapentry.S that saves enough information about the user environment to return to it, sets up the kernel environment, pushes the arguments to syscall() and calls syscall() directly. Once syscall() returns, set everything up for and execute the sysexit instruction. You will also need to add code to kern/init.c to set up the necessary model specific registers (MSRs). Section 6.1.2 in Volume 2 of the AMD Architecture Programmer's Manual and the reference on SYSENTER in Volume 2B of the Intel reference manuals give good descriptions of the relevant MSRs. You can find an implementation of wrmsr to add to inc/x86.h for writing to these MSRs here.
> 
> Finally, lib/syscall.c must be changed to support making a system call with sysenter. Here is a possible register layout for the sysenter instruction:
> ```javascript
>	eax                - syscall number
>	edx, ecx, ebx, edi - arg1, arg2, arg3, arg4
>	esi                - return pc
>	ebp                - return esp
>	esp                - trashed by sysenter
> ```	
> GCC's inline assembler will automatically save registers that you tell it to load values directly into. Don't forget to either save (push) and restore (pop) other registers that you clobber, or tell the inline assembler that you're clobbering them. The inline assembler doesn't support saving %ebp, so you will need to add code to save and restore it yourself. The return address can be put into %esi by using an instruction like leal after_sysenter_label, %%esi.
>
> Note that this only supports 4 arguments, so you will need to leave the old method of doing system calls around to support 5 argument system calls. Furthermore, because this fast path doesn't update the current environment's trap frame, it won't be suitable for some of the system calls we add in later labs.
> 
> You may have to revisit your code once we enable asynchronous interrupts in the next lab. Specifically, you'll need to enable interrupts when returning to the user process, which sysexit doesn't do for you.

留做

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





