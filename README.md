## **StrawberryBASIC**
ver 0.01a7

![StrawberryBASIC](https://gitee.com/steven-yang-blue/strawberry-basic/raw/master/STRAWBERRY-BASIC.png)

原创BASIC方言和解释器

已经实现了基本表达式、赋值、算数/逻辑/比较运算、If-ElseIf-Else语句、While循环、Do-Loop循环、For循环
Break语句、Continue语句、Sub定义过程、Exit Sub语句、Function自定义函数、Return语句

- 语法和教程请参考仓库中的最新版本用户手册pdf，该手册假定你至少已经粗略地学过微软公司的Visual Basic（或者.Net版）、TurboBASIC等其中一种BASIC的实现
- BASIC语言是Beginners' All-purpose Symbolic Instruction Code的缩写，一般译为“初学者通用符号指令代码”，主打易学性和易用性
- StrawberryBASIC语言是BASIC语言的方言，但大体上是一样的
- 编译后只有60KB！

开发环境：

- Ubuntu/GCC/Flex/Bison
- 如果有意愿，也容易移植到Windows平台、LLVM的Clang编译器等

编译安装（Ubuntu）：

- make flex
- make bison
- make compile

用标准输入流运行脚本（Ubuntu）：

- ./BASIC < ../test/Testcase1.basic
- ./BASIC < ../test/Testcase2.basic
- ./BASIC < ../test/Testcase3.basic
- ./BASIC < ../test/Testcase4.basic

开发

- Steven Yang（杨泰然）@steven-yang-blue

LOGO

- Steven Yang（杨泰然）@steven-yang-blue

