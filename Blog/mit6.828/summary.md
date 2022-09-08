# 1. xv6启动流程

## 1.1 BIOS 

BIOS 程序不需要由谁加载，本身便固化在ROM只读存储器中；开机的一瞬间 cs:ip 便被初始化为0xf000 : 0xfff0。开机的时候处于实模式，其等效地址为0xffff0。

BIOS 程序做了以下事情：

• POST 自检，检验主板，内存，各类外设

• 对设备进行初始化

• 建立中断表、构建 BIOS 数据区、加载中断服务程序

• 权力交接给 MBR

BIOS 最后一项工作便是加载启动盘最开始那个扇区里面的引导程序到0x7c00，然后跳去执行

> MBR 和 Bootloader 的区别
> 
> 广义上可以认为他们是相同的，Bootlaoder 表示主引导程序代码，而MBR （Main Boot Record 主引导记录区） 包括 Bootloader 和  DPT（Disk Partition Table硬盘分区表），DPT记录了磁盘的分区情况，由并列的四项组成。所以硬盘最多可以有四个分区。

























