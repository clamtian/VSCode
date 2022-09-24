# 1. xv6启动流程

## 1.1 BIOS 

内存低 1M 的顶部 64K 都是分配给 BIOS 的。

BIOS 程序不需要由谁加载，本身便固化在ROM只读存储器中；开机的一瞬间 cs:ip 便被初始化为0xf000 : 0xfff0。开机的时候处于实模式，其等效地址为0xffff0。

BIOS 程序做了以下事情：

• POST 自检，检验主板，内存，各类外设

• 对设备进行初始化

• 权力交接给 MBR

BIOS 最后一项工作便是加载启动盘最开始那个扇区里面的引导程序到0x7c00，然后跳去执行。

## 1.2 Bootloader

xv6 并没有实际构造 MBR 结构，最开始的那个扇区写入的是 bootlock，bootlock是由 bootasm.S 和 bootmain.c 两文件经过编译得到的，这两个文件组成的程序就充当了 MBR 和 Bootloader 的角色。

bootasm.S 主要做了一件事：进入保护模式，主要分三步

* 打开 A20：地址总线可以使用 32 根，寻址能力达到 4G；
* 构建加载 GDT：构建临时 GDT 并将其地址加载到 GDTR 寄存器中；
* 设置 CR0 寄存器：将 CR0 寄存器的 PE 位置1 ，开启保护模式。

执行完这三步后，执行 bootmain.c 程序，将操作系统内核加载到内存中，整个内核就是一个 elf 文件，内核在磁盘上，所以要先读取磁盘。

加载内核分为两步：首先将 elf 文件头加载到 0x10000 的位置，然后将 elf 文件加载到 0x100000 的位置处。接下来就将权利移交给内核，由内核完成接下来的启动。

## 1.3 Kernel

启动的后半部分都交由内核来做，主要三件事：

* 开启分页机制
* 对系统的各个部分进行初始化
* BSP 启动 APs
  
Bootloader 后首先会进入到 entry.S，entry.S 做了三件事

* 创建临时页表，将虚拟地址的[0,4M)和[0xF0000000,0xF0000000+4M)映射到物理地址[0,4M)
* 加载页表，将临时页表的位置加载到 CR3 寄存器
* 将 CR0 寄存器 PG 位置 1 ，表示开启分页。

至此下面的所有地址全部使用虚拟地址，而在完成正式页表的创建之前，只会使用[0,4M)和[0xF0000000,0xF0000000+4M)的地址。

完成这些后就会跳转到 kern/init.c 中的 i386_init 函数。

在这之前，有必要看一下当前的内存分布图：

![avatar](./image/lab1内存使用情况.png)

i386_init 函数主要是对各种机制的初始化，我们会在下面详细展开。


# 2.内存管理部分

首先是内存的初始化，主要在 mem_init() 函数中完成。主要分以下几步：

* 首先将内存中空闲部分的大小所占页数 npages 计算出来，这就是最大物理内存（不超过 2GB）；
* 然后初始化内核页表 kern_pgdir；
* 初始化 pages 数组，该数组保存了相应页面的信息（是否被占用）；
* 用头插法初始化 page_free_list 链表，记录相应空页信息；
* 然后是映射内核一部分空间到用户内存中，分别是 UPAGES 映射 pages，KSTACKTOP - KSTKSIZE 映射 bootstack，这是内核栈 32KB，然后就是将内核部分 KERNBASE 映射到物理地址 0 处，这样低地址处的 BIOS 和 Kernel Code 等内存区域全部被搬运到了高地址处，低地址就留给用户使用。
  
至此，我们需要彻底告别物理内存，今后只需要用虚拟内存。





























> MBR 和 Bootloader 的区别
> 
> 广义上可以认为他们是相同的，Bootlaoder 表示主引导程序代码，而MBR （Main Boot Record 主引导记录区） 包括 Bootloader 和  DPT（Disk Partition Table硬盘分区表），DPT记录了磁盘的分区情况，由并列的四项组成。所以硬盘最多可以有四个分区。




本文大部分都不会讲，留待后面相应的部分。这里主要讲一下多处理器的初始化。

```javascript
int i386_init(void)
{
mp_init(); // detect other processors 检测其他CPU
boot_aps(); // start other processors 启动其他APs
mpmain(); // finish this processor's setup 完成该BSP的启动
}
```

首先是 mpinit，mpinit会在MP 配置表（MP Configuration Table）中获取 CPU 的配置信息，MP 配置表存在于 BIOS 内存区域。读取 MP 配置表后将相关信息填进一个全局的 CPU 数据结构。然后 boot_aps 会启动其它 CPU。
