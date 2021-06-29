# Exercise 1
>Familiarize yourself with the assembly language materials available on the 6.828 reference page.

```Javascript
The target architecture is assumed to be i8086
[f000:fff0]    0xffff0:	ljmp   $0xf000,$0xe05b
0x0000fff0 in ?? ()
+ symbol-file obj/kern/kernel
```
从开机的首段代码可以看出，IBM PC最开始在物理地址0x000ffff0处执行，它位于为BIOS保留的64KB区域的最顶部。



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
    

