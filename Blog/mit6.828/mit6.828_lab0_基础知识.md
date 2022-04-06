# 0.常用指令
这一章节主要记录在整个实验中常用的一些指令，由于实验完成时间跨度比较长，所以专门开辟了一个章节用来记录，省去查找的时间。

1. 运行操作系统
   * `make qume` 
   * `make qemu-nox` 无窗口
   * `make run-x` 运行x程序
   * 
2. 调试操作系统
   * `make qemu-gdb` && `make gdb`
   * `si` 运行一步

3.提交流程
   * `git commit -am "lab3 a"` 先提交
   * `make handin` 上交
   * `make grade` 打分


# 1.汇编知识

1.esp是栈顶指针寄存器，ebp是栈底指针寄存器。

```javascript
     +--------------------+ <---- EBP 栈底
     |        ...         |     " - 4 
     |      data 3        |     " - 8 
     |      data 2        |     " - 12 
     |      data 1        |     " - 16  <---- ESP 栈顶 
     +--------------------+             
```

2.指令指针EIP、IP (Instruction Pointer)是存放下次将要执行的指令在代码段的偏移量

3.ret指令：将栈顶值存入IP寄存器中，实现执行指令的跳转

