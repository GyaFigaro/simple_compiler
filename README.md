# simple_compiler
a self-finished simple compiler as a project in the Compilation Principle course, in C and C++ 

文件说明：

input_code.txt        输入代码样例（源程序）
token.txt                 词法分析输出的符号（token）串
pre_symbol.txt        词法分析输出的临时符号表
productions_out     语法分析输出的产生式序列
midcode.txt            中间代码
extended.txt           扩展符号表
result.txt                 输出的最终结果，即源程序的对应汇编代码
LR1相关                   该文件夹存放了LR1的相关内容，包括状态机、分析表、文法文件等


操作说明：

按照运行程序exe输出提示后

必须先输入1进行语法分析

输入1后会显示输入源程序文件名
样例：input_code.txt

接下来依次输入功能对应的数字即可

如不按照编译顺序操作的话会报错
