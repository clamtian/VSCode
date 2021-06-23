mit6.828的系统启动分为两个部分，分别是BIOS和Boot Loader的启动。主机通电时，首先启动BIOS，BIOS将带领CPU识别并加载主板上的重要硬件和集成元件，如硬盘、显卡、声卡以及各种接口，然后按照预设顺序读取存储器上操作系统的引导文件Boot Loader。在我们的实验中，Boot Loader程序会在编译成可执行文件后被放在模拟硬盘的第一个扇区。

# 1.BIOS启动流程

BIOS(Basic Input Output System)"基本输入输出系统"，它是一组固化到主板上一个ROM芯片上的程序，使用汇编语言编写。PC通电后，CPU马上就从地址0xFFFF0处开始执行指令，这个地址在系统BIOS的地址范围内，将BIOS程序加载到内存中执行，BIOS接下来的作用包括机器自检，对系统进行初始化，识别并加载主板上的重要硬件和集成元件，如硬盘、显卡、声卡以及各种接口，然后按照**预设顺序**读取存储器上操作系统的引导文件Boot Loader，储存Boot Loader的存储器可以是软盘、硬盘、CD-ROM甚至是网络输入，可以在BIOS的设置页面更改读取Boot Loader的搜索顺序。

那么在这其中就有一个问题，那就是，BIOS程序是固化到自己的ROM芯片中的，那CPU是如何在刚通电的情况下就能读取到BIOS程序并且运行的？再者，上段的说法也有些问题，“CPU马上就从地址0xFFFF0处开始执行指令，这个地址在系统BIOS的地址范围内”，那地址0xFFFF0指的是什么地址？是外加内存条RAM的内存地址还是储存BIOS程序的ROM地址？这又涉及到现代PC的内存地址是如何编排的。我也是查了很多资料才摸到一些门道，下面的说法糅合了我从网上搜索的资料和自己的整合以及猜测，**不保证正确**，仅为缕清自己的思路。

在计算机通电完成之后，CPU会自动将其CS寄存器设定为0xFFFF，IP寄存器设定为0x0000。关于CS寄存器和IP寄存器的介绍可以看文末的链接6，简单来说CS是代码段寄存器，IP是指令指针寄存器(相当于偏移地址)，而CPU在访问内存的时候访问的是物理内存地址，而这个物理地址就是由CS和IP这两个寄存器中的值合成的(物理地址=段地址*16+偏移地址)。所以如果将CS寄存器设定为0xFFFF，IP寄存器设定为0x0000，那么这时的物理地址就=0xFFFF*16+0x0000=0xFFFF0，而在0xFFFF0存放的是什么呢？其实是一条无条件转移指令JMP，这个JMP会将程序跳转到BIOS真正的入口点。至于跳转到哪里，那么不同的BIOS会有不同的跳转地址。



```javascript

```
1.https://www.jianshu.com/p/af9d7eee635e
2.https://blog.csdn.net/rongwenbin/article/details/18962057
3.https://baike.baidu.com/item/BIOS%E5%BD%B1%E5%AD%90%E5%86%85%E5%AD%98
4.https://blog.csdn.net/hwxlovezy/article/details/53454305
5.https://www.cnblogs.com/wsw-seu/p/10475956.html
6.https://blog.csdn.net/weixin_37924880/article/details/78644659