# Exercise1
>Familiarize yourself with the assembly language materials available on the 6.828 reference page.

```Javascript
The target architecture is assumed to be i8086
[f000:fff0]    0xffff0:	ljmp   $0xf000,$0xe05b
0x0000fff0 in ?? ()
+ symbol-file obj/kern/kernel
```
从开机的首段代码可以看出，IBM PC最开始在物理地址0x000ffff0处执行，它位于为BIOS保留的64KB区域的最顶部。



# Exercise2
>Use GDB's si (Step Instruction) command to trace into the ROM BIOS for a few more instructions, and try to guess what it might be doing. 
```Javascript
(gdb) si
[f000:e05b]    0xfe05b:	cmpl   $0x0,%cs:0x6ac8
0x0000e05b in ?? ()
```
开机之后执行的第一条命令便是跳转到[f000:e05b]位置，即16*0xf000 + 0xe05b = 0xfe05b的位置，这个内存的位置便是我们的BIOS程序的入口。

# Exercise3
>Set a breakpoint at address 0x7c00, which is where the boot sector will be loaded. Continue execution until that breakpoint. Trace through the code in *boot/boot.S*, using the source code and the disassembly file *obj/boot/boot.asm* to keep track of where you are. Also use the *x/i* command in GDB to disassemble sequences of instructions in the boot loader, and compare the original boot loader source code with both the disassembly in *obj/boot/boot.asm* and GDB.

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