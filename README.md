## **StrawberryBASIC**
ver 0.01a16

![StrawberryBASIC](https://gitee.com/steven-yang-blue/strawberry-basic/raw/master/STRAWBERRY-BASIC.png)

原创现代化编程语言StrawberryBASIC和它的解释器（建设中）

A BRAND-NEW INTERPRETOR OF BASIC PROGRAMMING LANGUAGE (UNDER CONSTRUCTION)


已经实现了基本表达式（算术/逻辑/比较运算）、赋值、重新赋值、If-ElseIf-Else语句、While循环、Do-Loop循环、For循环、
Break语句、Continue语句、Sub定义过程、Exit Sub语句、Function自定义函数、Return语句、面向对象开发进行中！

- 语法和教程请参考仓库中的最新版本用户手册pdf，该手册假定你至少已经粗略地学过微软公司的Visual Basic（或者.Net版）、FreeBASIC或PowerBASIC等其中一种BASIC的实现
- BASIC语言是Beginners' All-purpose Symbolic Instruction Code的缩写，一般译为“初学者通用符号指令代码”，主打易学性和易用性
- StrawberryBASIC语言是BASIC语言的方言，但大体上是一样的
- Ubuntu下生产模式编译后只有100KB！

开发环境：

- Ubuntu >= 20.04.1
- GCC/Flex/Bison
- 本软件使用ANSI C标准开发。如果有意愿，也容易移植到Windows平台、LLVM的Clang编译器等其他平台或编译器。

编译安装（Ubuntu）：

- make flex
- make bison
- make production

配置系统变量

- export LD_LIBRARY_PATH=编译好的lib路径:$LD_LIBRARY_PATH

用标准输入流运行脚本（Ubuntu）：

- ./BASIC < ../test/Testcase1.basic
- ./BASIC < ../test/Testcase2.basic
- ./BASIC < ../test/Testcase3.basic
- ./BASIC < ../test/Testcase4.basic
- ./BASIC < ../test/Testcase5.basic
- ./BASIC < ../test/Testcase6.basic
- ./BASIC < ../test/Testcase7.basic
- ./BASIC < ../test/Testcase8.basic

开发/手册/美术：

- Steven Yang（杨泰然）@steven-yang-blue

感谢无私的开源组件贡献者

ACKNOWLEDGEMENT TO THE WORD-WIDE OPEN-SOURCE CONTRIBUTORS

- Przemo Nowaczyk（波兰）
- Dane Larsen（美国）
- Seungyoung Kim 金承英（韩国）Team 团队

PS. Generally and hopefully, I push the project to Gitee and GitHub synchronously. But in the view of the Internet content arguments between China and US, leading to the off and on network, GitHub version would probably be delayed of releasing. If you are eager to checkout the latest update, you may visit Gitee (a Chinese-mainland GitHub) version which the URL is given below.

https://gitee.com/steven-yang-blue/strawberry-basic
