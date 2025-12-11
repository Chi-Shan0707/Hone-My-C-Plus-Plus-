## Linux 中 gcc 与 gdb 的关系

- `gcc`：GNU Compiler Collection，编译器，用来把 C/C++ 源代码（`.c`/`.cpp`）编译成可执行文件或目标文件。
- `gdb`：GNU Debugger，调试器，用来在程序运行时设置断点、单步执行、查看变量、回溯调用栈等。

二者的典型配合方式：

1. 用 `gcc` 带调试信息编译：

	```bash
	gcc -g main.c -o main
	# 或 C++：g++ -g main.cpp -o main
	```

	选项 `-g` 会在可执行文件中加入调试符号（行号、变量名等），方便 `gdb` 使用。

2. 用 `gdb` 调试由 `gcc/g++` 生成的程序：

	```bash
	gdb ./main
	```

	在 gdb 里可以：

	- `break 行号` / `break 函数名`：设置断点
	- `run`：开始运行程序
	- `next` / `step`：单步执行
	- `print 变量名`：查看变量值
	- `backtrace`：查看调用栈

简单理解：**gcc 负责编译生成“能被调试”的程序，gdb 负责在运行阶段“拆开”这个程序，帮你观察和控制它的执行过程。**


## Some thoughts
* 竞赛算法偏理论悬浮，工程落地重实效。实践出真知，“做中学”方能串联知识、落地优化，比如，加速矩阵运算的数据结构与访存方式
相关数据放同一内存块、连续读写、块化(tile)存储、并行协同访问等；CUDA、deepseek 同理，性能来自良好数据布局+算法。