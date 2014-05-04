YetAnotherRegularExpressionEngine
=================================
最近看了《编译器设计（第二版）》，是莱斯大学的Keith D. Cooper与Linda Torczon合著的，非常精彩。
我决定在阅读的同时按照书上的指导做一些编码，加深我对知识的理解。
这个正则表达式引擎就是由此产生的第一个作品。

版本号按照alpha、beta、gamma的希腊字母顺序排列，不同版本平行维护，现在先放出alpha版。
alpha版只支持 * 与 | 两个符号，往后会逐渐加入更完善的功能。
以下3个功能是我考虑在接下来增加的，其中的一个或多个会选入beta版的功能。

- 逃逸字符
- 分组功能
- 将alpha版采用的NFA改为用DFA

所有代码通过以下环境的编译、运行测试：
- Microsoft Visual Studio Express 2013 for Windows Desktop
- gcc version 4.8.1 (tdm64-2)

本项目采用 MIT 授权，use it at your own risk。
