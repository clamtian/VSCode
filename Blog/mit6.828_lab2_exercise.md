# Exercise 1
> In the file kern/pmap.c, you must implement code for the following functions (probably in the order given).
>
> `boot_alloc()`
> 
> `mem_init()` (only up to the call to `check_page_free_list(1)`)
> 
> `page_init()`
> 
> `page_alloc()`
> 
> `page_free()`
>
> `check_page_free_list()` and `check_page_alloc()` test your physical page allocator. You should boot JOS and see whether `check_page_alloc()` reports success. Fix your code so that it passes. You may find it helpful to add your own assert()s to verify that your assumptions are correct.

我们首先来实现`boot_alloc()`，这个函数的作用是申请n个字节的连续内存，并且返回所申请空间的开始地址：

```Javascript
static void *
boot_alloc(uint32_t n)
{
	static char *nextfree;
	char *result;

	if (!nextfree) {
		extern char end[];
		nextfree = ROUNDUP((char *) end, PGSIZE);
	}

	if(n == 0) return nextfree;
	result = nextfree;
	nextfree = ROUNDUP(nextfree + n, PGSIZE);
	if(nextfree <= (char *)0xf0400000) return result;

	nextfree = result;
	panic("boot_alloc : out of memory, nothing changed, return NULL...\n");

	return NULL;
}
```
`nextfree`指针首先被初始化为紧挨着 Kernel Code 之后的一个页帧。`ROUNDUP`的作用是使`nextfree`指针式中对准4KB(一个页帧)的整数倍。由于我们目前只映射了虚拟地址 0xf0000000:0xf0400000，所以一旦超出 0xf0400000 我们需要提醒错误。

接下来我们需要在`mem_init()`函数中添加代码，完成的功能是分配一块内存，用来存放一个`struct PageInfo`的数组，数组中的每一个`PageInfo`代表内存当中的一页。操作系统内核就是通过这个数组来追踪所有内存页的使用情况的。

```JavaScript
  	pages = (struct PageInfo *)boot_alloc(npages * sizeof(struct PageInfo));
	memset(pages, 0, npages * sizeof(struct PageInfo));
```

这部分已经在解析篇中详细介绍过了，这里不再赘述。

接下来是`page_init()`函数，这个子函数的功能包括初始化`pages`数组和`pages_free_list`链表，这个链表中存放着所有空闲页的信息。我们可以到这个函数的定义处具体查看，整个函数是由一个循环构成，它会遍历所有内存页所对应的在`npages`数组中的`PageInfo`结构体，并且根据这个页当前的状态来修改这个结构体的状态，如果页已被占用，那么要把`PageInfo`结构体中的`pp_ref`属性置一；如果是空闲页，则要把这个页加入`pages_free_list`链表中。

```JavaScript
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
		if(i >= index_IOhole_begin && i < index_alloc_end){
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
其中`index_IOhole_begin`和`index_alloc_end`分别是IOhole的起始地址以及`nextfree`指针所在的地址，在解析篇中我们说过，目前为止内存只有开头的一个页帧以及IOhole和内核段被使用，所以这些区域不应该被插入到`page_free_list`链表中。

`page_alloc()`函数功能是分配一个物理页，然后返回这个物理页所对应的`PageInfo`结构体。

```JavaScript
void
struct PageInfo *
page_alloc(int alloc_flags)
{
	struct PageInfo *res = page_free_list;
	if(!res) return NULL;
	page_free_list = page_free_list->pp_link;
	res->pp_link = NULL;
	if(alloc_flags & ALLOC_ZERO){
		memset(page2kva(res), '\0', PGSIZE);
	}
	return res;
}
```

我们首先从`free_page_list`中取出一个空闲页的`PageInfo`结构体，然后修改链表表头，接着需要修改取出的空闲页的`PageInfo`结构体信息，初始化该页的内存。

`page_free()`函数的功能就是把一个页的`PageInfo`结构体插入回`page_free_list`空闲页链表，代表回收了这个页。

```JavaScript
void
page_free(struct PageInfo *pp)
{
	if(!pp || pp->pp_ref != 0 || pp->pp_link){
		panic("page_free : Invalid address, nothing changed...\n");
		return;
	}
	pp->pp_link = page_free_list;
	page_free_list = pp;
}
```

我们首先需要修改被回收的页的`PageInfo`结构体的相应信息，然后把该结构体插入回`page_free_list`空闲页链表。



# Exercise 2
>Use GDB's si (Step Instruction) command to trace into the ROM BIOS for a few more instructions, and try to guess what it might be doing. 
```Javascript
(gdb) si
[f000:e05b]    0xfe05b:	cmpl   $0x0,%cs:0x6ac8
0x0000e05b in ?? ()
```
开机之后执行的第一条命令便是跳转到[f000:e05b]位置，即16*0xf000 + 0xe05b = 0xfe05b的位置，这个内存的位置便是我们的BIOS程序的入口。

# Exercise 3
>Set a breakpoint at address 0x7c00, which is where the boot sector will be loaded. Continue execution until that breakpoint. Trace through the code in *boot/boot.S*, using the source code and the disassembly file *obj/boot/boot.asm* to keep track of where you are. Also use the *x/i* command in GDB to disassemble sequences of instructions in the boot loader, and compare the original boot loader source code with both the disassembly in *obj/boot/boot.asm* and GDB.
>
>Trace into *bootmain()* in *boot/main.c*, and then into *readsect()*. Identify the exact assembly instructions that correspond to each of the statements in *readsect()*. Trace through the rest of *readsect()* and back out into *bootmain()*, and identify the begin and end of the for loop that reads the remaining sectors of the kernel from the disk. Find out what code will run when the loop is finished, set a breakpoint there, and continue to that breakpoint. Then step through the remainder of the boot loader.

```JavaScript
(gdb) b *0x7c00
Breakpoint 1 at 0x7c00
(gdb) c
Continuing.
[   0:7c00] => 0x7c00:	cli    

Breakpoint 1, 0x00007c00 in ?? ()
(gdb) info register
eax            0xaa55              43605
ecx            0x0                 0
edx            0x80                128
ebx            0x0                 0
esp            0x6f20              0x6f20
ebp            0x0                 0x0
esi            0x0                 0
edi            0x0                 0
eip            0x7c00              0x7c00
eflags         0x202               [ IF ]
cs             0x0                 0
ss             0x0                 0
ds             0x0                 0
es             0x0                 0
fs             0x0                 0
gs             0x0                 0
(gdb) si
[   0:7c01] => 0x7c01:	cld    
0x00007c01 in ?? ()
(gdb) si
[   0:7c02] => 0x7c02:	xor    %ax,%ax
0x00007c02 in ?? ()
(gdb) si
[   0:7c04] => 0x7c04:	mov    %ax,%ds
0x00007c04 in ?? ()
(gdb) si
[   0:7c06] => 0x7c06:	mov    %ax,%es
0x00007c06 in ?? ()
(gdb) x /6i 0x7c06
=> 0x7c06:	mov    %ax,%es
   0x7c08:	mov    %ax,%ss
   0x7c0a:	in     $0x64,%al
   0x7c0c:	test   $0x2,%al
   0x7c0e:	jne    0x7c0a
   0x7c10:	mov    $0xd1,%al
```
*b *address* 是在address这个位置设置一个断点；*c* 是运行到下一个断点；*info register* 查看寄存器内容；*si* 执行下一条指令；*x /Ni address* 查看包含address在内的接下来的N条指令(但不执行)。
## Question
> At what point does the processor start executing 32-bit code? What exactly causes the switch from 16- to 32-bit mode?

在 *boot.S* 中将处理器从实模式转换到保护模式，这之后处理器便从 16 位切换到了 32 位，代码如下：
```JavaScript
# Switch from real to protected mode, using a bootstrap GDT
# and segment translation that makes virtual addresses 
# identical to their physical addresses, so that the 
# effective memory map does not change during the switch.
lgdt    gdtdesc
movl    %cr0, %eax
orl     $CR0_PE_ON, %eax
movl    %eax, %cr0
```

> What is the last instruction of the boot loader executed, and what is the first instruction of the kernel it just loaded?

Boot Loader 的最后一条指令是跳转到 Kernel 入口的指令。
```JavaScript
((void (*)(void)) (ELFHDR->e_entry))();
```

从 *obj/boot/boot.asm* 中可以看到对应的汇编语句是
```JavaScript
((void (*)(void)) (ELFHDR->e_entry))();
7d81:       ff 15 18 00 01 00       call   *0x10018
```

可以看到最后是跳转到 0x10018 这个地址的值处执行，那么这个地址存储的内容是什么呢？
```JavaScript
(gdb) x /1x 0x10018
0x10018:	0x00000000
(gdb) b *0x7d81
Breakpoint 2 at 0x7d81
(gdb) c
Continuing.
=> 0x7d81:	call   *0x10018

Breakpoint 2, 0x00007d81 in ?? ()
(gdb) x /1x 0x10018
0x10018:	0x0010000c
```
可以看到，在 Boot Loader 刚运行时，还未向 0x10018 地址写入数据；等运行之后，0x10018 中便被写进了 0x10000c，而这正是 Kernel 的入口地址。

# Exercise 5
> Trace through the first few instructions of the boot loader again and identify the first instruction that would "break" or otherwise do the wrong thing if you were to get the boot loader's link address wrong. Then change the link address in *boot/Makefrag* to something wrong, run `make clean`, recompile the lab with make, and trace into the boot loader again to see what happens. Don't forget to change the link address back and `make clean` again afterward!

修改 *boot/Makefrag* 中的VMA 0x7c00 为一个错误地址 0x7c20，观察接下来的运行错误：

```JavaScript
[f000:fff0]    0xffff0:	ljmp   $0xf000,$0xe05b
0x0000fff0 in ?? ()
+ symbol-file obj/kern/kernel
(gdb) c
Continuing.

Program received signal SIGTRAP, Trace/breakpoint trap.
[   0:7c2d] => 0x7c2d:	ljmp   $0x8,$0x7c52
0x00007c2d in ?? ()
(gdb) 

```
可以看到，JOS在运行到 `ljmp   $0x8,$0x7c52` 这个指令时出现了错误，这是因为我们把Boot Loader的VMA改成了 0xx7c20。本来VMA是 0x7c00 时，这句指令是 `ljmp   $0x8,$0x7c32`，意为去地址 0x7c32 寻找指令，而在我们把VMA改成了 0xx7c20之后，Boot Loader的LMA没有变，还是 0x7c00，想要寻找的指令还是存放在 0x7c32 处，这时Boot Loader去 0x7c52 处肯定是找不到正确指令的。

# Exercise 6
> Reset the machine (exit QEMU/GDB and start them again). Examine the 8 words of memory at 0x00100000 at the point the BIOS enters the boot loader, and then again at the point the boot loader enters the kernel. Why are they different? What is there at the second breakpoint? (You do not really need to use QEMU to answer this question. Just think.)

```JavaScript
[f000:fff0]    0xffff0:	ljmp   $0xf000,$0xe05b
0x0000fff0 in ?? ()
+ symbol-file obj/kern/kernel
(gdb) b *0x7c00
Breakpoint 1 at 0x7c00
(gdb) c
Continuing.
[   0:7c00] => 0x7c00:	cli    

Breakpoint 1, 0x00007c00 in ?? ()
(gdb) x /8wx 0x00100000
0x100000:	0x00000000	0x00000000	0x00000000	0x00000000
0x100010:	0x00000000	0x00000000	0x00000000	0x00000000
(gdb) b *0x10000c
Breakpoint 2 at 0x10000c
(gdb) c
Continuing.
The target architecture is assumed to be i386
=> 0x10000c:	movw   $0x1234,0x472

Breakpoint 2, 0x0010000c in ?? ()
(gdb) x /8wx 0x00100000
0x100000:	0x1badb002	0x00000000	0xe4524ffe	0x7205c766
0x100010:	0x34000004	0x2000b812	0x220f0011	0xc0200fd8
(gdb) 
```

可以看到，在运行 Boot Loader之前(进入地址 0x7c00之前)，0x100000 内存处是没有内容的。而Boot Loader运行完成之后，这是Kernel已经被加载进内存中，所以此时 0x100000 处已经有了内容。

# Exercise 7
> Use QEMU and GDB to trace into the JOS kernel and stop at the movl %eax, %cr0. Examine memory at 0x00100000 and at 0xf0100000. Now, single step over that instruction using the stepi GDB command. Again, examine memory at 0x00100000 and at 0xf0100000. Make sure you understand what just happened.
> 
> What is the first instruction after the new mapping is established that would fail to work properly if the mapping weren't in place? Comment out the movl %eax, %cr0 in kern/entry.S, trace into it, and see if you were right.

理解开启分页的效果，`mov %eax, %cr0` (在地址 0x100025 处) 这条指令开启分页：

```JavaScript
(gdb) b *0x100025
Breakpoint 3 at 0x100025
(gdb) c
Continuing.
=> 0x100025:	mov    %eax,%cr0

Breakpoint 3, 0x00100025 in ?? ()
(gdb) x/8x 0x00100000
0x100000:	0x1badb002	0x00000000	0xe4524ffe	0x7205c766
0x100010:	0x34000004	0x2000b812	0x220f0011	0xc0200fd8
(gdb) x/8x 0xf0100000
0xf0100000 <_start-268435468>:	0x00000000	0x00000000	0x00000000	0x00000000
0xf0100010 <entry+4>:	0x00000000	0x00000000	0x00000000	0x00000000
(gdb) si
=> 0x100028:	mov    $0xf010002f,%eax
0x00100028 in ?? ()
(gdb) x/8x 0x00100000
0x100000:	0x1badb002	0x00000000	0xe4524ffe	0x7205c766
0x100010:	0x34000004	0x2000b812	0x220f0011	0xc0200fd8
(gdb) x/8x 0xf0100000
0xf0100000 <_start-268435468>:	0x1badb002	0x00000000	0xe4524ffe	0x7205c766
0xf0100010 <entry+4>:	0x34000004	0x2000b812	0x220f0011	0xc0200fd8
```

可以看到，在未开启分页之前，高地址 0xf0100000 处无内容，而开启分页之后，虚拟地址 0xf0000000:0xf0400000 映射到了物理地址 0x00000000:0x00400000 处，所以 虚拟地址 0xf0100000 的内容与虚拟地址 0x00100000 相同。

如果注释掉 *kernel/entry.S* 中的 `movl %eax, %cr0` 这条指令，因为高地址没有映射，执行完 `jmp *%eax` 后跳转到 eax 寄存器保存的值所在地址 0xf010002c 时会报如下错误：

```JavaScript
qemu: fatal: Trying to execute code outside RAM or ROM at 0xf010002c
```

# Exercise 8
>  We have omitted a small fragment of code - the code necessary to print octal numbers using patterns of the form "%o". Find and fill in this code fragment.

将 *lib/printfmt.c* 中的以下内容：

```JavaScript
case 'o':
         // Replace this with your code.
         putch('X', putdat);
         putch('X', putdat);
         putch('X', putdat);
         break;
```

修改为：

```JavaScript
case 'o':
         num = getuint(&ap, lflag);
         base = 8;
         goto number;
```

这样，便完成了八进制的输出。

## Question
> Explain the interface between *printf.c* and *console.c*. Specifically, what function does *console.c* export? How is this function used by printf.c?

*kern/console.c* 这个文件实现了cputchar，getchar等函数，而cputchar函数最终调用了cga_putc函数来完成显示功能。*lib/printfmt.c* 这个文件实现了vprintfmt函数，vprintfmt函数是一个最精简的原始printf函数，它会被printf, sprintf, fprintf等函数调用，是一个既被内核使用也被用户使用的函数。而cprintf是在 *kern/printf.c* 中实现的，它调用了vprintfmt函数，而vprintfmt函数调用了putch函数作为参数，putch函数最终调用了console.c中的cputchar。即 cprintf --> vprintfmt --> putch --> cputchar --> cga_putc。

> Explain the following from *console.c*:

```JavaScript
if (crt_pos >= CRT_SIZE) {
        int i;
        memmove(crt_buf, crt_buf + CRT_COLS, (CRT_SIZE - CRT_COLS) * sizeof(uint16_t));
        for (i = CRT_SIZE - CRT_COLS; i < CRT_SIZE; i++)
                crt_buf[i] = 0x0700 | ' ';
        crt_pos -= CRT_COLS;
}
```

这段代码的作用是实现屏幕滚动一行。

> Trace the execution of the following code step-by-step:

```JavaScript
   int x = 1, y = 3, z = 4;
   cprintf("x %d, y %x, z %d\n", x, y, z);
```

> In the call to cprintf(), to what does fmt point? To what does ap point?
> List (in order of execution) each call to cons_putc, va_arg, and vcprintf. For cons_putc, list its argument as well. For va_arg, list what ap points to before and after the call. For vcprintf list the values of its two arguments.

在调用cprintf函数时，fmt指向的是第一个字符串参数，ap指向的是fmt后面的参数。

> Run the following code.
```JavaScript
   unsigned int i = 0x00646c72;
   cprintf("H%x Wo%s", 57616, &i);
```
> What is the output? Explain how this output is arrived at in the step-by-step manner of the previous exercise.
> The output depends on that fact that the x86 is little-endian. If the x86 were instead big-endian what would you set i to in order to yield the same output? Would you need to change 57616 to a different value?

输出是 `He110 World` ，因为 57616 = 0xe110，所以前半部分输出是 `He110`，而 i = 0x00646c72 被当做字符串输出，所以它会被输出为 `'r'=(char)0x72 'l'=(char)0x6c 'd'=(char)0x64`。若在大端字节序的机器上，输出会变成 `He110 Wodlr`。

> In the following code, what is going to be printed after 'y='? (note: the answer is not a specific value.) Why does this happen?
```JavaScript
   cprintf("x=%d y=%d", 3);
```
如果fmt里面指定的格式化字符串数目大于实际参数数目，因为缺少参数，而由可变参数的方式知道会打印第一个参数之上的栈里面的4字节内容。

> Let's say that GCC changed its calling convention so that it pushed arguments on the stack in declaration order, so that the last argument is pushed last. How would you have to change cprintf or its interface so that it would still be possible to pass it a variable number of arguments?

如果GCC参数入栈方式改为从左往右，则可能需要加一个表示参数个数的参数传到cprintf函数中以获取可变参数。
    
# Exercise 9
> Determine where the kernel initializes its stack, and exactly where in memory its stack is located. How does the kernel reserve space for its stack? And at which "end" of this reserved area is the stack pointer initialized to point to?

在 *kern/entry.S* 文件中为栈分配了空间，大小是8*4KB = 32KB：

```JavaScript
# Set the stack pointer
movl	$(bootstacktop),%esp
........
........
.data
###################################################################
# boot stack
###################################################################
	.p2align	PGSHIFT		# force page alignment
	.globl		bootstack
bootstack:
	.space		KSTKSIZE
	.globl		bootstacktop   
bootstacktop:

```

栈的结束位置即栈顶是在*obj/kern/kernel.asm*中设置的，为0xf0110000：

```JavaScript
	# Set the stack pointer
	movl	$(bootstacktop),%esp
f0100034:	bc 00 00 11 f0       	mov    $0xf0110000,%esp
```

# Exercise 10
> To become familiar with the C calling conventions on the x86, find the address of the test_backtrace function in *obj/kern/kernel.asm*, set a breakpoint there, and examine what happens each time it gets called after the kernel starts. How many 32-bit words does each recursive nesting level of test_backtrace push on the stack, and what are those words?

在 *obj/kern/kernel.asm* 中，可以找到test_backtrace函数：

```JavaScript
// Test the stack backtrace function (lab 1 only)
void
test_backtrace(int x)
{
f0100040:	f3 0f 1e fb          	endbr32 
f0100044:	55                   	push   %ebp
f0100045:	89 e5                	mov    %esp,%ebp
f0100047:	56                   	push   %esi
f0100048:	53                   	push   %ebx
f0100049:	e8 7e 01 00 00       	call   f01001cc <__x86.get_pc_thunk.bx>
f010004e:	81 c3 ba 12 01 00    	add    $0x112ba,%ebx
f0100054:	8b 75 08             	mov    0x8(%ebp),%esi
	cprintf("entering test_backtrace %d\n", x);
f0100057:	83 ec 08             	sub    $0x8,%esp
f010005a:	56                   	push   %esi
f010005b:	8d 83 18 08 ff ff    	lea    -0xf7e8(%ebx),%eax
f0100061:	50                   	push   %eax
f0100062:	e8 38 0a 00 00       	call   f0100a9f <cprintf>
	if (x > 0)
f0100067:	83 c4 10             	add    $0x10,%esp
f010006a:	85 f6                	test   %esi,%esi
f010006c:	7e 29                	jle    f0100097 <test_backtrace+0x57>
		test_backtrace(x-1);
f010006e:	83 ec 0c             	sub    $0xc,%esp
f0100071:	8d 46 ff             	lea    -0x1(%esi),%eax
f0100074:	50                   	push   %eax
f0100075:	e8 c6 ff ff ff       	call   f0100040 <test_backtrace>
f010007a:	83 c4 10             	add    $0x10,%esp
	else
		mon_backtrace(0, 0, 0);
	cprintf("leaving test_backtrace %d\n", x);
f010007d:	83 ec 08             	sub    $0x8,%esp
f0100080:	56                   	push   %esi
f0100081:	8d 83 34 08 ff ff    	lea    -0xf7cc(%ebx),%eax
f0100087:	50                   	push   %eax
f0100088:	e8 12 0a 00 00       	call   f0100a9f <cprintf>
}
```

在test_backtrace函数处打上断点，运行观察系统执行到test_backtrace函数处发生了什么：

```JavaScript
(gdb) b test_backtrace
Breakpoint 1 at 0xf0100040: file kern/init.c, line 13.
(gdb) c
Continuing.
The target architecture is assumed to be i386
=> 0xf0100040 <test_backtrace>:	endbr32 

Breakpoint 1, test_backtrace (x=5) at kern/init.c:13
13	{
(gdb) i r
eax            0x0                 0
ecx            0x3d4               980
edx            0x3d5               981
ebx            0xf0111308          -267316472
esp            0xf010ffdc          0xf010ffdc
ebp            0xf010fff8          0xf010fff8
esi            0x10094             65684
edi            0x0                 0
eip            0xf0100040          0xf0100040 <test_backtrace>
eflags         0x46                [ PF ZF ]
cs             0x8                 8
ss             0x10                16
ds             0x10                16
es             0x10                16
fs             0x10                16
gs             0x10                16
(gdb) c
Continuing.
=> 0xf0100040 <test_backtrace>:	endbr32 

Breakpoint 1, test_backtrace (x=4) at kern/init.c:13
13	{
(gdb) i r
eax            0x4                 4
ecx            0x3d4               980
edx            0x3d5               981
ebx            0xf0111308          -267316472
esp            0xf010ffbc          0xf010ffbc
ebp            0xf010ffd8          0xf010ffd8
esi            0x5                 5
edi            0x0                 0
eip            0xf0100040          0xf0100040 <test_backtrace>
eflags         0x92                [ AF SF ]
cs             0x8                 8
ss             0x10                16
ds             0x10                16
es             0x10                16
fs             0x10                16
gs             0x10                16
(gdb) x/8x 0xf010ffbc
0xf010ffbc:	0xf010007a	0x00000004	0x00000005	0x00000000
0xf010ffcc:	0xf010004e	0xf0111308	0x00010094	0xf010fff8
```

可以发现在一次递归调用test_backtrace的过程中，esp由指向 0xf010ffdc 变成了指向 0xf010ffbc，所以每次调用test_backtrace函数都会向栈内压进 0x20 = 32byte 的数据，即8个4字节数据。

# Exercise 11
> Implement the backtrace function as specified above. Use the same format as in the example, since otherwise the grading script will be confused. When you think you have it working right, run make grade to see if its output conforms to what our grading script expects, and fix it if it doesn't. After you have handed in your Lab 1 code, you are welcome to change the output format of the backtrace function any way you like.

mon_backtrace函数的原型在*kern/monitor.c* 文件中：

```JavaScript
int
mon_backtrace(int argc, char **argv, struct Trapframe *tf)
{
	// Your code here.
	uint32_t eip;
	int i = 0;
	uint32_t* ebp = (uint32_t *)read_ebp();
	while(ebp){
		eip = *(ebp + 1);
		cprintf("ebp %x eip %x arg ", ebp, eip);
		for(i = 0; i < 5; ++i){
			cprintf("%x ", *(ebp + i + 2));
		}
		cprintf("\n");
		ebp = (uint32_t *)*(ebp);
	}
	return 0;
}
```

完成以上内容后重启JOS：

```JavaScript
6828 decimal is 15254 octal!
entering test_backtrace 5
entering test_backtrace 4
entering test_backtrace 3
entering test_backtrace 2
entering test_backtrace 1
entering test_backtrace 0
ebp f010ff18 eip f01000a5 arg 0 0 0 f010004e f0111308          //mon_backtrace()
ebp f010ff38 eip f010007a arg 0 1 f010ff78 f010004e f0111308   //test_backtrace(0)
ebp f010ff58 eip f010007a arg 1 2 f010ff98 f010004e f0111308   //test_backtrace(1) 
ebp f010ff78 eip f010007a arg 2 3 f010ffb8 f010004e f0111308   //test_backtrace(2)
ebp f010ff98 eip f010007a arg 3 4 0 f010004e f0111308          //test_backtrace(3)
ebp f010ffb8 eip f010007a arg 4 5 0 f010004e f0111308          //test_backtrace(4)
ebp f010ffd8 eip f01000fc arg 5 1aac 640 0 0                   //test_backtrace(5)
ebp f010fff8 eip f010003e arg 3 1003 2003 3003 4003            //i386_init()
leaving test_backtrace 0
leaving test_backtrace 1
leaving test_backtrace 2
leaving test_backtrace 3
leaving test_backtrace 4
leaving test_backtrace 5
Welcome to the JOS kernel monitor!
Type 'help' for a list of commands.
```

可以看到在mon_backtrace()函数处打印的eip寄存器内的值为 0xf01000a5，而这正是调用完函数mon_backtrace后一个指令的地址，可以在*obj/kern/kernel.asm*文件中找到调用函数指令：

```JavaScript
		mon_backtrace(0, 0, 0);
f0100097:	83 ec 04             	sub    $0x4,%esp
f010009a:	6a 00                	push   $0x0
f010009c:	6a 00                	push   $0x0
f010009e:	6a 00                	push   $0x0
f01000a0:	e8 0c 08 00 00       	call   f01008b1 <mon_backtrace>
f01000a5:	83 c4 10             	add    $0x10,%esp
f01000a8:	eb d3                	jmp    f010007d <test_backtrace+0x3d>
```

可以看到，在调用完函数mon_backtrace后，下一个指令的地址正是 0xf01000a5。可以在test_backtrace的调用过程中找到更多类似的结论。

# Exercise 12
> Modify your stack backtrace function to display, for each eip, the function name, source file name, and line number corresponding to that eip.

这个练习要增强mon_backtrace函数的功能，使其可以打印每个 eip 的函数名、文件名和行号，这项功能主要依靠*kern/kdebug.c*中的debuginfo_eip函数来完成。在debuginfo_eip函数中还需要我们添加查询行号的代码：

```JavaScript
// Your code here.
	stab_binsearch(stabs, &lline, &rline, N_SLINE, addr);

	if(lline <= rline){
		info->eip_line = stabs[rline].n_desc;
	}else{
		info->eip_line = -1;
	}
```

完成debuginfo_eip函数后，我们就可以在mon_backtrace中调用它：

```JavaScript
while(ebp){
		........
		struct Eipdebuginfo debug_info;
		debuginfo_eip(eip, &debug_info);
		cprintf("\t%s:%d: %.*s+%d\n",
			debug_info.eip_file, debug_info.eip_line, debug_info.eip_fn_namelen,
			debug_info.eip_fn_name, eip - debug_info.eip_fn_addr);
	}
```

重启系统，可以得到如下输出：

```JavaScript
ebp f0110f18 eip f01000a5 arg 0 0 0 f010004e f0112308 
	     kern/init.c:0: test_backtrace+101
ebp f0110f38 eip f010007a arg 0 1 f0110f78 f010004e f0112308 
	     kern/init.c:16: test_backtrace+58
ebp f0110f58 eip f010007a arg 1 2 f0110f98 f010004e f0112308 
	     kern/init.c:16: test_backtrace+58
ebp f0110f78 eip f010007a arg 2 3 f0110fb8 f010004e f0112308 
	     kern/init.c:16: test_backtrace+58
ebp f0110f98 eip f010007a arg 3 4 0 f010004e f0112308 
	     kern/init.c:16: test_backtrace+58
ebp f0110fb8 eip f010007a arg 4 5 0 f010004e f0112308 
	     kern/init.c:16: test_backtrace+58
ebp f0110fd8 eip f01000fc arg 5 1aac 640 0 0 
	     kern/init.c:39: i386_init+82
ebp f0110ff8 eip f010003e arg 3 1003 2003 3003 4003 
	     kern/entry.S:83: <unknown>+0

```