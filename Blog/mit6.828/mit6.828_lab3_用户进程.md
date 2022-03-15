# 0.前言
在将lab2的代码merge到lab3的时候，发生了一点小插曲，导致lab2完成的代码在lab3中失效了，所幸最后解决了这个bug，所以这里专门开一个小节用来记录一下。
在应用git命令
```JavaScript
git merge lab2
```
合并分支之后，在之前lab2中完成的代码便被合并到了lab3中，但我在进行测试时，发现lab2的测试无法通过，会报以下错误：
```JavaScript
6828 decimal is 15254 octal!
Physical memory: 131072K available, base = 640K, extended = 130432K
end = :f0190000
kernel panic at kern/pmap.c:164: PADDR called with invalid kva 00000000
Welcome to the JOS kernel monitor!
Type 'help' for a list of commands.
```

重点是`kernel panic at kern/pmap.c:164: PADDR called with invalid kva 00000000`这一句，这句话的意思是说宏函数`PADDR`中传入的是一个零值，它对应于`pmap.c`中给`kern_pgdir`赋值的那行代码(下面第三行)：
```JavaScript
kern_pgdir = (pde_t *) boot_alloc(PGSIZE);
memset(kern_pgdir, 0, PGSIZE);
kern_pgdir[PDX(UVPT)] = PADDR(kern_pgdir) | PTE_U | PTE_P;
```
从上面的代码中可以看到，`PADDR`中传入的是页目录表指针`kern_pgdir`，我们通过`boot_alloc`给这个指针分配了一个页的空间，但是为何这个指针值为0，我们将指针值和地址打印出来：
```JavaScript
kern_pgdir = (pde_t *) boot_alloc(PGSIZE);
cprintf("kern_pgdir = %x\n", kern_pgdir);
cprintf("kern_pgdir address = %x\n", &kern_pgdir);
memset(kern_pgdir, 0, PGSIZE);
kern_pgdir[PDX(UVPT)] = PADDR(kern_pgdir) | PTE_U | PTE_P;

********************************************output***********************************

kern_pgdir = f0190000
kern_pgdir address = f019000c
```
可以看到，`kern_pgdir`自己的地址在指向地址的高位，所以后面再用`memset`初始化空间时便将`kern_pgdir`的值也一并初始化了。所以在`boot_alloc`中将初始输出值往高位拉个偏移，使`kern_pgdir`自己的地址在指向地址的低位，这样问题便能解决。

```JavaScript
kern_pgdir = f0191000
kern_pgdir address = f019000c
```

问题是解决了，可引发了一个问题，`boot_alloc`初始的输出值应该是kernel代码所占空间的后面(下图)，而`kern_pgdir`是在kernel代码中定义的，所以它的地址不应该超出kernel代码空间的范围。按照上面的输出结果来看，kernel代码空间应该是在地址0xf0190000处终止，而`kern_pgdir`的地址是在
0xf019000c处，这显然超过了0xf0190000。内核中定义的指针竟然超出了内核空间！这显然是不可思议的，但是本人目前的知识储备不足以支持我解决这个疑惑，所以暂记在此处，待日后再来填这个坑。
![avatar](./image/lab1内存使用情况.png)


# 1.函数回顾

在lab1和lab2中，我们实现了很多函数，在进行lab3之前，我们先对这些函数进行一下总结。

```JavaScript
宏
KERNBASE 0xF0000000 //被映射的基准地址，即物理地址+KERNBASE=虚拟地址
KADDR(pa)           //宏函数，将物理地址pa转换为虚拟地址，即pa+KERNBASE
PADDR(kva)          //宏函数，将虚拟地址kva转换为物理地址，即kva-KERNBASE
PTE_ADDR(pte)       //取出page table entry或者page directory entry中的地址
PDX(la)             //取出虚拟地址la中包含的page directory index

函数
//将pp转换为物理地址
static inline phtsaddr_t page2pa(struct PageInfo *pp)
//将pp转换为虚拟地址
static inline phtsaddr_t page2kva(struct PageInfo *pp)
//将物理地址pa转换为页指针
static inline struct PageInfo* pa2page(physaddr_t pa)
//初始的内存分配器，返回n字节的虚拟内存地址，不初始化内存；n为0时，返回下一可用页的地址
static void* boot_alloc(uint32_t n)   
//正式的页分配器，分配一页的内存，如果(alloc_flags & ALLOC_ZERO)，则初始化分配的空间
struct PageInfo* page_alloc(int alloc_flags)
//释放pp指向的页
void page_free(struct PageInfo *pp)
//给定虚拟地址va和page directory pgdir，返回虚拟地址在这个pgdir下的page table entry
//若page table不存在且create == true，则新建page table
pte_t* pgdir_walk(pde_t *pgdir, const void *va, int create)
//将虚拟地址[va, va + size] 映射至物理地址[pa, pa + size]上，pgdir为使用的page directory
static void boot_map_region(pde_t *pgdir, uintptr_t va, size_t size, physaddr_t pa, int perm)
//返回虚拟地址va所对应的页指针
//若pte_store不为0，将va对应的page table entry储存至其中
struct PageInfo* page_lookup(pde_t *pgdir, void *va, pte_t **pte_store)
//删除虚拟地址va的映射
void page_remove(pde_t *pgdir, void *va)
//将虚拟地址va映射至物理页地址pp
int page_insert(pde_t *pgdir, struct PageInfo *pp, void *va, int perm)
```
以上这些函数会贯穿lab3的始终，所以可以时不时的回顾一下。

在lab3中，我们将实现操作系统的一些基本功能，来实现用户环境下的进程的正常运行。你将会加强JOS内核的功能，为它增添一些重要的数据结构，用来记录用户进程环境的一些信息；创建一个单一的用户环境，并且加载一个程序运行它。你也需要让JOS内核能够完成用户环境所作出的任何系统调用，以及处理用户环境产生的各种异常。

# 2.用户环境

新添加的文件`inc/env.h`里面包含了JOS内核的有关用户环境(User Environment)的一些基本定义。用户环境指的就是一个应用程序运行在系统中所需要的一个上下文环境，操作系统内核使用数据结构 `Env` 来记录每一个用户环境的信息。在这个实验中，我们将会先创建一个用户环境，但是之后我们会把它设计成能够支持多用户环境，即多个用户程序并发执行的系统。

在 `kern/env.c` 文件中我们看到，操作系统一共维护了三个重要的和用户环境相关的全局变量：

```JavaScript
struct Env *envs = NULL;		// All environments
struct Env *curenv = NULL;		// The current env
static struct Env *env_free_list;	// Free environment list
```

JOS内核使用 Env结构体来追踪用户进程。其中 `envs`变量是指向所有进程的链表的指针，其操作方式跟lab2的`pages`类似，`env_free_list`是空闲的进程结构链表。注意下，在早起的JOS实验中，`pages`和`envs`都是用的双向链表，现在的版本用的单向链表操作起来更加简单和清晰。

JOS系统启动之后，`envs`指针便指向了一个 `Env` 结构体链表，表示系统中所有的用户环境。在我们的设计中，JOS内核将支持同一时刻最多 `NENV`(1024) 个活跃的用户环境，尽管这个数字要比真实情况下任意给定时刻的活跃用户环境数要多很多。系统会为每一个活跃的用户环境在`envs`链表中维护一个 `Env` 结构体。

JOS内核也把所有未执行的`Env`结构体，用`env_free_list`连接起来。这种设计方式非常方便进行用户环境`env`的分配和回收。

内核也会把 `curenv` 指针指向在任意时刻正在执行的用户环境的 `Env` 结构体。在内核启动时，并且还没有任何用户环境运行时，`curenv`的值为NULL。

接下来看一下`Env`结构体的定义：

```JavaScript
struct Env {
	struct Trapframe env_tf;	// Saved registers
	struct Env *env_link;		// Next free Env
	envid_t env_id;				// Unique environment identifier
	envid_t env_parent_id;		// env_id of this env's parent
	enum EnvType env_type;		// Indicates special system environments
	unsigned env_status;		// Status of the environment
	uint32_t env_runs;			// Number of times environment has run

	// Address space
	pde_t *env_pgdir;			// Kernel virtual address of page dir
};
```

进程结构体 Env 各字段定义如下：

* env_tf： 当进程停止运行时用于保存寄存器的值，比如当发生中断切换到内核环境运行了或者切换到另一个进程运行的时候需要保存当前进程的寄存器的值以便后续该进程继续执行。
* env_link：指向空闲进程链表 env_free_list 中的下一个 Env 结构。
* env_id： 进程ID。因为进程ID是正数，所以符号位是0，而中间的21位是标识符，标识在不同的时间创建但是却共享同一个进程索引号的进程，最后10位是进程的索引号，要用envs索引进程管理结构 Env 就要用 ENVX(env_id)。
* env_parent_id： 进程的父进程ID。
* env_type：进程类型，通常是 ENV_TYPE_USER，后面实验中可能会用到其他类型。
* env_status：进程状态，进程可能处于下面几种状态
  * ENV_FREE：标识该进程结构处于不活跃状态，存在于 env_free_list 链表。
  * ENV_RUNNABLE: 标识该进程处于等待运行的状态。
  * ENV_RUNNING: 标识该进程是当前正在运行的进程。
  * ENV_NOT_RUNNABLE: 标识该进程是当前运行的进程，但是处于不活跃的状态，比如在等待另一个进程的IPC。
* ENV_DYING: 该状态用于标识僵尸进程。在实验4才会用到这个状态，实验3不用。
* env_pgdir：用于保存进程页目录的虚拟地址。

接下来我们需要对管理用户环境的结构进行初始化，进程管理结构`envs`对应的1024个Env结构体在物理内存中紧接着pages存储。如下图`nextfree`指向的位置：

![avatar](./image/lab2内存使用情况4.png)

用户进程运行路径如下所示：

```JavaScript
start (kern/entry.S)
i386_init (kern/init.c)
    cons_init
    mem_init
    env_init
    trap_init (still incomplete at this point)
    env_create
        env_alloc
            env_setup_vm
        load_icode
            region_alloc
    env_run
        env_pop_tf
};
```

进程初始化流程主要包括：

* 给`NENV`个Env结构体在内存中分配空间，并将 envs 结构体的物理地址映射到 从 `UENV` 所指向的线性地址空间，该线性地址空间允许用户访问且只读，所以页面权限被标记为`PTE_U`。

* 调用`env_init`函数初始化envs，将 `NENV` 个进程管理结构Env通过`env_link`串联起来，注意，`env_free_list`要指向第一个 Env，所以这里要用倒序的方式。在`env_init`函数中调用了`env_init_percpu`函数，加载新的全局描述符表。
  
* 初始化好了`envs`和`env_free_list`后，接着调用 `ENV_CREATE(user_hello, ENV_TYPE_USER)` 创建用户进程。`ENV_CREATE`是`kern/env.h`中的宏定义，展开就是调用的 `env_create`,只是参数设置成了 `env_create(_binary_obj_user_hello_start, ENV_TYPE_USER)`。`env_create`也是我们要实现的函数，它的功能就是先调用`env_alloc`函数分配好Env结构，初始化Env的各个字段值(如`env_id`，`env_type`，`env_status`以及`env_tf`的用于存储寄存器值的字段，运行用户程序时会将 `env_tf` 的字段值加载到对应的寄存器中)，为该用户进程分配页目录表并调用`load_icode`函数加载程序代码到内存中。
  
  * `env_alloc`调用`env_setup_vm`函数分配好页目录的页表，并设置页目录项和`env_pgdir`字段)。
  * `load_icode`函数则是先设置cr3寄存器切换到该进程的页目录`env_pgdir`，然后通过`region_alloc`分配每个程序段的内存并按segment将代码加载到对应内存中，加载完成后设置 `env_tf->tf_eip`为Elf的`e_entry`，即程序的初始执行位置。
  
* 加载完程序代码后，万事俱备，调用 `env_run(e)` 函数开始运行程序。如果当前有进程正在运行，则设置当前进程状态为`ENV_RUNNABLE`，并将需要运行的进程e的状态设置为`ENV_RUNNING`，然后加载e的页目录表地址 `env_pgdir` 到cr3寄存器中，调用 `env_pop_tf(struct Trapframe *tf)` 开始执行程序e。
  
* `env_pop_tf`其实就是将栈指针esp指向该进程的`env_tf`，然后将 `env_tf` 中存储的寄存器的值弹出到对应寄存器中，最后通过 `iret` 指令弹出栈中的元素分别到 EIP, CS, EFLAGS 到对应寄存器并跳转到 CS:EIP 存储的地址执行(当使用`iret`指令返回到一个不同特权级运行时，还会弹出堆栈段选择子及堆栈指针分别到SS与SP寄存器)，这样，相关寄存器都从内核设置成了用户程序对应的值，EIP存储的是程序入口地址。
  
* `env_id`的生成规则很有意思，注意一下在`env_free`中并没有重置`env_id`的值，这就是为了用来下一次使用这个env结构体时生成一个新的`env_id`，区分之前用过的`env_id`，从generation的生成方式就能明白了。

## 关于ELF文件

在`load_icode()`函数中，我们要对ELF文件进行解析，之前在lab1中我们讲过了ELF文件的结构，这里再简单介绍一下。

在`load_icode()`函数中我们没有加载真的可执行文件，因为还没有文件系统。这个Lab使用的ELF文件是通过连接器嵌入到内核这个可执行文件中的。ELF文件有多个ProgHdr，对应多个区块，每个区块都有指定好的虚拟地址和长度，我们只需要从ELF中读取出这些信息，把数据拷贝到相应位置就可以。

有内存空间才能执行指令，在拷贝ELF文件中的各个部分到指定位置之前，需要通过已有的分配器给进程足够多的page，用于存放ELF文件镜像。光是ELF文件中的内容还不够，进程执行还需要栈，我们也要给进程栈分配空间，并映射到指定虚拟地址。

**ELF文件结构**

```JavaScript
struct Elf {
	uint32_t e_magic;	// must equal ELF_MAGIC
	uint8_t e_elf[12];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
};
```

















这个lab的核心就是要搞清楚 `x86` 的内存管理机制，强烈建议大家阅读Lab讲义、Lab搭配的[指导书](https://pdos.csail.mit.edu/6.828/2018/readings/i386/c05.htm) 和Lab已经提供的代码，对照着本文来进行理解。

先来看看 `x86` 的内存管理架构：

```

           Selector  +--------------+         +-----------+
          ---------->|              |         |           |
                     | Segmentation |         |  Paging   |
Software             |              |-------->|           |---------->  RAM
            Offset   |  Mechanism   |         | Mechanism |
          ---------->|              |         |           |
                     +--------------+         +-----------+
        Virtual(Logical)              Linear                 Physical
```
我们代码中的 C 指针就是虚拟地址(virtual address，也称逻辑地址logical address)中的 offset，通过描述符表和段选择子(selector)，通过分段机制转换为线性地址(linear address)，因为JOS中设置的段基址为0，所以线性地址就等于offset，也就是我们代码中的c指针。在未开启分页之前，线性地址就是物理地址。而在我们开启分页之后，线性地址经过 CPU 的MMU部件的页式转换得到物理地址(physical address)，而物理地址就是数据真正存在内存上的地址。

从现实上来看，一个线性地址就是一个32位的 `uint32_t` 类型。具体组成如下图：

![avatar](./image/linear_address.png)

开启分页后，当处理器碰到一个线性地址后，它的MMU部件会把这个地址分成 3 部分，分别是页目录索引(DIR)、页表索引(PAGE)和页内偏移(OFFSET)， 这 3 个部分把原本 32 位的线性地址分成了 10+10+12 的 3 个片段，虚拟地址向物理地址转换的过程如下：

![avatar](./image/page_translation.png)

我们来详述上图的流程：如何通过一个32位的线性地址找到它所对应的物理地址呢？首先，我们需要一个 `pgdir` 指针，这个指针指向上图中的页目录表 Page Directory 的首地址。而线性地址的 DIR 部分又指向了这个 Page Directory 中的一个页目录表项 DIR ENTRY，这个 DIR ENTRY 又指向了一个页表 Page Table 的首地址，同样，PAGE 部分又指向了这个 Page Table 中的一个页表项 ENTRY，而这个 ENTRY 就指向了一块物理地址，这块物理地址的大小是 4KB，称为页帧(page frame，也称页面，更具体地，和这块物理地址相对应的虚拟地址称为虚拟页Virtual Page，这块物理地址称为物理页Physical Page)，OFFSET最终指向了这个页帧中的一个地址。以上就完成了线性地址向物理地址的转换。由于 DIR 和 PAGE 长度为10，所以一个32位的线性地址最终能表示的物理地址最大为 $2^{10} \times 2^{10} \times 4KB = 4GB$。而通常我们说每个用户进程虚拟地址空间为4GB，其实就是每个进程都有一个页目录表  `pgdir` ，进程运行时将页目录地址装载到CR3寄存器中，从而每个进程最大可以用4GB内存。在JOS中，为了简单起见，只用了一个页目录表，整个系统的线性地址空间4GB是被内核和所有其他的用户程序所共用的。

>## entry格式

![avatar](./image/entry.png)

ENTRY不仅包含了地址信息，还包含了很多其他有用的信息。DIR ENTRY 和 PG TBL ENTRY 的格式相同，除了前20位包含地址信息，它们还包含：

* P：Present，判断对应物理页面是否存在，存在为1，否则为0；
* W：Write，判断对应的物理页面是否可写，可写为1，不可写为0；
* U：User，定义页面的访问者所需要的权限，为1则所有user都可访问，为0则需要特权用户才能访问；
* WT：Write-though，定义对应物理页面的写方式，为1则为直写，为0则为回写
* CD：Cache-disabled，1为禁用缓存，0为不禁用；
* D：Ditry，数据是否被修改，1为是，0为否；
* A：Accessed，数据最近是否被访问；
* AVL：Available，是否可以被系统程序所使用


# 3.页面管理

通过页表和虚拟地址我们找到了相对应的物理地址，现在我们来看看JOS在形式上是怎么来管理这块内存的。在lab1中我们通过一个手写的页表实现了将 0xf0000000:0xf0400000 这4MB虚拟地址映射至物理地址 0x00000000:0x00400000，在lab2中我们需要将映射拓展至256MB。

JOS在 *kern/pmap.h* 中提供了两个宏函数 `KADDR(pa)` 和 `PADDR(va)`，前者将物理地址转换为虚拟地址，后者将虚拟地址转换为物理地址。不过注意，我们之前只完成了4MB的内存映射，所以这里的虚拟地址 `va` 的有效范围是 0xf0000000:0xf0400000，物理地址 `pa` 的有效范围是 0x00000000:0x00400000，除此之外的地址会报错。

```JavaScript
/* This macro takes a kernel virtual address -- an address that points above
 * KERNBASE, where the machine's maximum 256MB of physical memory is mapped --
 * and returns the corresponding physical address.  It panics if you pass it a
 * non-kernel virtual address.
 */
#define PADDR(kva) _paddr(__FILE__, __LINE__, kva)

/* This macro takes a physical address and returns the corresponding kernel
 * virtual address.  It panics if you pass an invalid physical address. */
#define KADDR(pa) _kaddr(__FILE__, __LINE__, pa)
```

上面代码注释中的 `KERNBASE` 定义在 *inc/memlayout.h* 中：

```JavaScript
// All physical memory mapped at this address
#define	KERNBASE	0xF0000000
```

操作系统必须要追踪记录哪些内存区域是空闲的，哪些是被占用的。JOS内核是以页(page)为最小粒度来管理内存的，它使用MMU来映射，保护每一块被分配出去的内存。JOS是通过链表来管理空闲物理内存块-->页帧(page frame)的，这个链表结构为 `struct PageInfo`，其定义在：

```JavaScript
struct PageInfo {
	// Next page on the free list.
	struct PageInfo *pp_link;

	// pp_ref is the count of pointers (usually in page table entries)
	// to this page, for pages allocated using page_alloc.
	// Pages allocated at boot time using pmap.c's
	// boot_alloc do not have valid reference count fields.

	uint16_t pp_ref;
};
```

这个 `PageInfo` 将所有未使用的页帧连起来，当需要分配内存时，将链表头部返回，并将链表头部更新为链表中的下一个元素。 `PageInfo` 中的 pp_ref 保存的是 `PageInfo` 对应物理页面的引用次数，当这个 pp_ref 为0时，就说明没有程序在使用这个页帧，所以这个物理页帧就应该被回收。

## 页面操作相关的宏

这些宏定义在 *inc/mmu.h* 中：

```JavaScript
// 线性地址分为如下三部分
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(la) --/ \--- PTX(la) --/ \---- PGOFF(la) ----/
//  \---------- PGNUM(la) ----------/

// 页号
#define PGNUM(la)   (((uintptr_t) (la)) >> PTXSHIFT)

// 页目录项索引(高10位)
#define PDX(la)     ((((uintptr_t) (la)) >> PDXSHIFT) & 0x3FF)

// 页表项索引（中间10位）
#define PTX(la)     ((((uintptr_t) (la)) >> PTXSHIFT) & 0x3FF)

// 页内偏移
#define PGOFF(la)   (((uintptr_t) (la)) & 0xFFF)

// 构造线性地址
#define PGADDR(d, t, o) ((void*) ((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// 页目录和页表的一些常量定义
#define NPDENTRIES  1024   //每个页目录的页目录项数目为1024
#define NPTENTRIES  1024   //每个页表的页表项数目也为1024

#define PGSIZE      4096   // 页大小为4096B，即4KB
#define PGSHIFT     12      // log2(PGSIZE)

#define PTSIZE      (PGSIZE*NPTENTRIES) // 一个页目录项映射内存大小，4MB
#define PTSHIFT     22      // log2(PTSIZE)

#define PTXSHIFT    12       
#define PDXSHIFT    22  

```

# 4.分页管理开启流程

回到启动内核的函数`i386_init`函数(在*kern/init.c*中)，在初始化控制台后进行内存管理初始化，即`mem_init`函数，所以`mem_init`函数是我们这次练习的入口。在内核刚开始运行时就会调用`mem_init`函数(在*kern/pmap.c*中)，对整个操作系统的内存管理系统进行一些初始化的设置，比如设定页表等等操作。

```JavaScript
void
mem_init(void)
{
	uint32_t cr0;
	size_t n;

	// Find out how much memory the machine has (npages & npages_basemem).
	i386_detect_memory();

	kern_pgdir = (pde_t *) boot_alloc(PGSIZE);
	memset(kern_pgdir, 0, PGSIZE);

	kern_pgdir[PDX(UVPT)] = PADDR(kern_pgdir) | PTE_U | PTE_P;

	pages = (struct PageInfo *)boot_alloc(npages * sizeof(struct PageInfo));
	memset(pages, 0, npages * sizeof(struct PageInfo));

	page_init();

	check_page_free_list(1);
	check_page_alloc();
	check_page();

	check_kern_pgdir();


	lcr3(PADDR(kern_pgdir));

	check_page_free_list(0);


	cr0 = rcr0();
	cr0 |= CR0_PE|CR0_PG|CR0_AM|CR0_WP|CR0_NE|CR0_MP;
	cr0 &= ~(CR0_TS|CR0_EM);
	lcr0(cr0);

	check_page_installed_pgdir();
}

```

下面进入这个函数，首先这个函数调用 `i386_detect_memory` 子函数，这个子函数会调用硬件，检测现在系统中有多少可用的内存空间。然后初始化2个变量 `npages` 和 `npages_basemem`，前者表示的是整个物理内存空间的页数，后者表示的是 0x00000~0xA0000 这部分内存空间的页数。

接着执行的代码为：

```JavaScript
	kern_pgdir = (pde_t *) boot_alloc(PGSIZE);
	memset(kern_pgdir, 0, PGSIZE);
```

其中`kern_pgdir`是一个指针，它是指向操作系统的页目录表的指针，操作系统之后工作在虚拟内存模式下时，就需要这个页目录表进行地址转换。我们为这个页目录表分配的内存大小空间为PGSIZE，即一个页的大小，并且首先把这部分内存清0。我们上面提到过，在JOS中，只用了一个页目录表，且一个页目录表的大小为 $1024 \times 4B = 4KB$(这个等式怎么来的？我们回顾一下页目录表和页表的组成，一个页目录表page directory包含1024个页目录项DIR ENTRY，而一个页目录项是一个4字节的变量)。

接着调用`boot_alloc`函数，这个函数是我们要首先实现的函数，在注释中有提到，它只是被用来暂时当做页分配器，之后我们使用的真实页分配器是`page_alloc`函数。而这个函数的核心思想就是维护一个静态变量`nextfree`，里面存放着下一个可以使用的空闲内存空间的**虚拟地址**，所以每次当我们想要分配n个字节的内存时，我们都需要修改这个变量的值。`nextfree` 的起始地址是紧挨着 Kernel Code 之后的一个页帧。执行完这两行代码后，内存布局变成了这样：

![avatar](./image/lab2内存使用情况.png)

由于 Boot Loader 和 ELF header 占据的空间是可以重用的，所以我把它们标成了蓝色，`boot_alloc`函数的实现我们放在exercise中来讲。这里接着来看下一条命令：

```JavaScript
	kern_pgdir[PDX(UVPT)] = PADDR(kern_pgdir) | PTE_U | PTE_P;
```

这指令就是在为页目录表添加第一个页目录表项。通过查看*memlayout.h*文件，我们可以看到，`UVPT`的定义是一个虚拟地址(线性地址) 0xef400000，从这个虚拟地址开始，存放的就是这个操作系统的页表`kern_pgdir`，所以我们必须把这个虚拟地址和页表的物理地址映射起来，`PADDR(kern_pgdir)`就是在计算`kern_pgdir`所对应的真实物理地址。具体可看下图：

![avatar](./image/first_entry.png)

下一条命令需要我们去补充，这条命令要完成的功能是分配一块内存，用来存放一个`struct PageInfo`的数组，数组中的每一个`PageInfo`代表内存当中的一页。操作系统内核就是通过这个数组来追踪所有内存页的使用情况的。

```JavaScript
  	pages = (struct PageInfo *)boot_alloc(npages * sizeof(struct PageInfo));
	memset(pages, 0, npages * sizeof(struct PageInfo));
```

一个`PageInfo`的大小是 8B，所以执行完上述代码后，内存布局变成了下图：

![avatar](./image/lab2内存使用情况2.png)

下一条指令我们将运行一个子函数`page_init()`，这个子函数的功能包括初始化`pages`数组和`pages_free_list`链表，这个链表中存放着所有空闲页的信息。我们可以到这个函数的定义处具体查看，整个函数是由一个循环构成，它会遍历所有内存页所对应的在`npages`数组中的`PageInfo`结构体，并且根据这个页当前的状态来修改这个结构体的状态，如果页已被占用，那么要把`PageInfo`结构体中的`pp_ref`属性置一；如果是空闲页，则要把这个页加入`pages_free_list`链表中。现在我们再来看一下我们内存的使用情况：

![avatar](./image/lab2内存使用情况3.png)

红色块是已经使用的内存，所以在它们相对应的`PageInof`中的`pp_ref`需要置一，并且这些`PageInfo`也不会被添加到`pages_free_list`链表中；而蓝色块相对应的`PageInof`中的`pp_ref`为0，因为它们是可用内存，并且需要添加到`pages_free_list`链表中。`page_init()`将会在exercise篇中实现。

初始化关于所有物理内存页的相关数据结构后，接下来的`check_page_free_list(1)`函数就是检查`page_free_list`链表的实现是否正确。`check_page_alloc()`用来检查`page_alloc()`和`page_free()`两个子函数是否能够正确运行，所以我们接下来要实现这两个子函数。

`page_alloc()`函数功能是分配一个物理页，然后返回这个物理页所对应的`PageInfo`结构体。而`page_free()`函数的功能就是把一个页的`PageInfo`结构体插入回`page_free_list`空闲页链表，代表回收了这个页。两个函数将会在exercise篇中实现。

至此，lab2的part1已经完成。








1. https://www.jianshu.com/p/752b7735a65b
2. https://pdos.csail.mit.edu/6.828/2018/readings/i386/c05.htm