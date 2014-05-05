YetAnotherRegularExpressionEngine
=================================
最近看了莱斯大学的Keith D. Cooper与Linda Torczon合著的《编译器设计（第二版）》，非常精彩。
我决定在阅读的同时按照书上的指导做一些编码，加深我对知识的理解。
这个正则表达式引擎就是由此产生的第一个作品。

版本号按照alpha、beta、gamma的希腊字母顺序排列，不同版本平行维护，现在先放出alpha版。
alpha版只支持 * 与 | 两个符号，其他字符都没有特殊含义，而RE中无法编码 * 与 | 两个字符。
beta版已成功发布并实现bug-free。beta版与alpha版的差别在于beta版引擎采用DFA状态机模型，运行效率已大幅提升。
在未来的gamma版将支持Dev-C++项目文件以及make文件，方便进行编译与测试。
gamma版还将支持最小化的DFA，运行效率将进一步提升。

同时，我正考虑在未来支持以下2个功能：

- 逃逸字符
- 分组功能

所有代码通过以下环境的编译、运行测试：
- Microsoft Visual Studio Express 2013 for Windows Desktop
- gcc version 4.8.1 (tdm64-2)

本项目采用 MIT 授权，use it at your own risk。
