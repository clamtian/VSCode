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

这个exercise是让我们阅读一些关于异常的资料，建议可以看一看，如果阅读英文比较吃力的话，可以去找一些博客来看。

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

















# Exercise 2 and Exercise 3

> 一些理论和操作性的东西，理论篇已经分析过了。

## Question
> Assuming that the following JOS kernel code is correct, what type should variable x have, uintptr_t or physaddr_t?
> ```JavaScript
>	mystery_t x;
>	char* value = return_a_pointer();
>	*value = 10;
>	x = (mystery_t) value;
> ```

由于我们在代码中使用的指针都是虚拟地址，所以 `x` 的类型应是 `uintptr_t`。

# Exercise 4
> In the file *kern/pmap.c*, you must implement code for the following functions.
>
> `pgdir_walk()`
> 
> `boot_map_region()`
>        
> `page_lookup()`
>        
> `page_remove()`
>        
> `page_insert()`
>	
> `check_page()`, called from `mem_init()`, tests your page table management routines. You should make sure it reports success before proceeding.

## `pgdir_walk()`

这个函数的原型是 `pgdir_walk(pde_t *pgdir, const void *va, int create)`，该函数的功能:给定一个页目录表指针 `pgdir` ，该函数应该返回线性地址`va`所对应的页表项指针。这个`pgdir`其实就是part1中我们初始化的`kern_pgdir`指针，它指向JOS中惟一的页目录表。所以在这里我们应该完成以下几个步骤：
* 通过页目录表求得这个虚拟地址页目录项地址 `pg_dir_entry`；
* 判断这个页目录项对应的页表是否已经在内存中；
* 如果在，计算这个页表的基地址`page_table`，然后返回`va`所对应页表项的地址 `&page_table[PTX(va)]``;
* 如果不在则分配新的页，并且把这个页的信息添加到页目录项`pg_dir_entry`中;
* 如果create为false，则返回NULL。

```JavaScript
pte_t *
pgdir_walk(pde_t *pgdir, const void *va, int create)
{
	pde_t pgdir_index = PDX(va);
	pde_t pgtab_index = PTX(va);
	pde_t pg_offset = PGOFF(va);
	pde_t *pgdir_add = &pgdir[pgdir_index];
	
	if(!(*pgdir_add & PTE_P)){
		if(!create) return NULL;
		struct PageInfo* new_page = page_alloc(ALLOC_ZERO);
		if(!new_page) return NULL;
		new_page->pp_ref++;
		*pgdir_add = (page2pa(new_page) | PTE_P | PTE_W | PTE_U);
	}
	pde_t *pgtab = KADDR(PTE_ADDR(*pgdir_add));
	return &pgtab[pgtab_index];
}
```

## `boot_map_region()`

函数原型 `static void boot_map_region(pde_t *pgdir, uintptr_t va, size_t size, physaddr_t pa, int perm)`，这个函数的功能是把虚拟地址空间范围`[va, va+size)`映射到物理空间`[pa, pa+size)`的映射关系加入到页表`pgdir`中。这个函数主要的目的是为了设置虚拟地址UTOP之上的地址范围，这一部分的地址映射是静态的，在操作系统的运行过程中不会改变，所以这些页的`PageInfo`结构体中的`pp_ref`域的值不会发生改变。

```JavaScript
static void
boot_map_region(pde_t *pgdir, uintptr_t va, size_t size, physaddr_t pa, int perm)
{
	uint32_t len = size / PGSIZE;
	for(uint32_t i = 0; i < len; ++i){
		pte_t *pg_table_entry = pgdir_walk(pgdir, (void *)va, 1);
		if(!pg_table_entry){
			panic("boot_map_region : something error happened...\n");
			return;
		}
		*pg_table_entry = pa | perm | PTE_P;
		pa += PGSIZE;
		va += PGSIZE;
	}
}
```

## `page_lookup()`
接下来继续完成`page_lookup()`函数，函数原型`struct PageInfo * page_lookup(pde_t *pgdir, void *va, pte_t **pte_store)`， 函数的功能为返回虚拟地址`va`所映射的物理页的`PageInfo`结构体的指针，如果`pte_store`参数不为0，则把这个物理页的页表项地址存放在`pte_stor`e中。

```JavaScript
struct PageInfo *
page_lookup(pde_t *pgdir, void *va, pte_t **pte_store)
{
	pte_t *pg_table_entry = pgdir_walk(pgdir, (void *)va, 0);
	if(!pg_table_entry || !(*pg_table_entry & PTE_P)) return NULL;

	struct PageInfo *page = pa2page(PTE_ADDR(*pg_table_entry));
	if(pte_store) *pte_store = pg_table_entry ;

	return page;
}
```

## `page_remove()`

`page_remove`函数，它的原型是`void page_remove(pde_t *pgdir, void *va)`，功能就是把虚拟地址va和物理页的映射关系删除。

```JavaScript
void
page_remove(pde_t *pgdir, void *va)
{
	pte_t *entry = NULL;
	struct PageInfo *page = page_lookup(pgdir, va, &entry);
	tlb_invalidate(pgdir, va);
	page_decref(page);
	*entry = 0;
}
```

## `page_insert()`

接下来继续查看`page_insert()`，函数原型 `page_insert(pde_t *pgdir, struct PageInfo *pp, void *va, int perm)`，把一个物理内存中页`pp`与虚拟地址`va`建立映射关系。
* 首先通过`pgdir_walk`函数求出虚拟地址`va`所对应的页表项；
* 修改`pp_ref`的值；
* 查看这个页表项，确定`va`是否已经被映射，如果被映射，则删除这个映射；
* 把`va`和`pp`之间的映射关系加入到页表项中。

```JavaScript
int
page_insert(pde_t *pgdir, struct PageInfo *pp, void *va, int perm)
{
	pte_t *pte = pgdir_walk(pgdir, va, 1);
	if (pte == NULL) {
	    return -E_NO_MEM;
	}
	if (*pte & PTE_P) {
	    if (PTE_ADDR(*pte) != page2pa(pp)) {
            page_remove(pgdir, va);
        }
	}
	if (PTE_ADDR(*pte) != page2pa(pp)) {
	    ++pp->pp_ref;
	}
	*pte = page2pa(pp) | perm | PTE_P;
	return 0;
}
```

# Exercise 5
> Fill in the missing code in `mem_init()` after the call to `check_page()`.
> Your code should now pass the `check_kern_pgdir()` and `check_page_installed_pgdir()` checks.

在`mem_init()` 中`check_page()`后添加以下内容：

```JavaScript
	boot_map_region(kern_pgdir, UPAGES, PTSIZE, PADDR(pages), PTE_U);
	boot_map_region(kern_pgdir, KSTACKTOP - KSTKSIZE, KSTKSIZE, PADDR(bootstack), PTE_W);
	boot_map_region(kern_pgdir, KERNBASE, 0xffffffff - KERNBASE, 0, PTE_W);
```

上述添加代码的具体功能我们在理论篇中已经分析过了，这里就不再赘述。

## Question

感谢[busui](https://blog.csdn.net/bysui/article/details/51471788)，题解大部分从他那里借鉴得来。

> What entries (rows) in the page directory have been filled in at this point? What addresses do they map and where do they point? 

|   Entry  |        Base Virtual Address          |    Points to (logically)   |
|   1024   |        0xffc00000                    |    kernel space            |
|   ...    |        ...                           |    kernel space            |
|   960    |        0xf0000000(KERNBASE)          |    kernel space            |
|   959    |        0xeffc0000(KSTACKTOP-KSTSIZE) |    bootstack               |
|   957    |        0xef400000(UVPT)              |    kern_pgdir              |
|   956    |        0xef000000(UPAGES)            |    pages                   | 

基本上就是我们在exercise5中完成的那几个映射区域。具体地以UPAGES为例，这部分映射的地址是pages数组对应的物理地址，UPAGES的地址为0xef000000，取其二进制的前十位11 1011 1100就得到其所在的entry，即956.

> We have placed the kernel and user environment in the same address space. Why will user programs not be able to read or write the kernel's memory? What specific mechanisms protect the kernel memory?

用户程序不能去随意修改内核中的代码，数据，否则可能会破坏内核，造成程序崩溃。正常的操作系统通常采用两个部件来完成对内核地址的保护，一个是通过段机制来实现的，但是JOS中的分段功能并没有实现。二就是通过分页机制来实现，通过把页表项中的 PTE_U 置0，那么用户态的代码就不能访问内存中的这个页。

> What is the maximum amount of physical memory that this operating system can support? Why?

pages数组映射到的线性地址UPAGES为4MB，也就是JOS利用一个大小为4MB的空间UPAGES来存放所有的页的PageInfo结构体信息，每个结构体的大小为8B，所以一共可以存放512K个PageInfo结构体，所以一共可以出现512K个物理页，每个物理页大小为4KB，自然总的物理内存占2GB。

> How much space overhead is there for managing memory, if we actually had the maximum amount of physical memory? How is this overhead broken down?

首先需要存放所有的PageInfo，需要4MB，需要存放页目录表kern_pgdir，大小为 $1024 \times 4B = 4KB$，还需要存放所有的页表，大小为 $2GB \div 4KB \times 4B = 2MB$。所以总的开销就是 $6MB + 4KB$。

> Revisit the page table setup in *kern/entry.S* and *kern/entrypgdir.c*. Immediately after we turn on paging, EIP is still a low number (a little over 1MB). At what point do we transition to running at an EIP above `KERNBASE`? What makes it possible for us to continue executing at a low EIP between when we enable paging and when we begin running at an EIP above `KERNBASE`? Why is this transition necessary?

在*entry.S*文件中有一个指令 `jmp *%eax`，这个指令要完成跳转，就会重新设置EIP的值，把它设置为寄存器eax中的值，而这个值是大于`KERNBASE`的，所以就完成了EIP从小的值到大于`KERNBASE`的值的转换。在`entry_pgdir`这个页表中，也把虚拟地址空间[0, 4MB)映射到物理地址空间[0, 4MB)上，所以当访问位于[0, 4MB)之间的虚拟地址时，可以把它们转换为物理地址。