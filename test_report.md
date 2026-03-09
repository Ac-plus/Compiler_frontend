# 编译原理 C--前端 测试分析报告

# 第三章 测试分析报告

## 3.1 测试用例

实验包含 4 个正确用例和 2 个错误用例。分别为：

* 00.txt
* 01.txt
* 02.txt
* 07.txt
* 08.txt（错误代码）
* 10.txt（错误代码）

## 3.2 词法分析测试

在词法分析测试中，我们将先对老师给出的 4 个正确用例和 2 个错误用例的 token 序列和符号表测试输出。然后，我们将使用自己的源码测试用例，对以下功能进行测试：

1. 有限自动机（NFA、DFA 等）的功能正确性；
2. 识别更全面的 KW、OP、SE、IDN 和 INT；
3. 负数识别功能正确性。

### 3.2.1 正确样例测试

对于给出的 4 个正确样例，测试输出如下。

#### 测试用例 1：00.txt（图 3-1）

```text
00_lexical.txt

1   void    <KW>
2   main    <IDN>
3   (       <SE>
4   )       <SE>
5   {       <SE>
6   return  <KW>
7   3       <INT>
8   ;       <SE>
9   }       <SE>
```

输出的符号表如图 3-2 所示。由此可见，符号表功能实现正确。在第一列中，输出了 IDN 的名称；在第二列中，输出了它的地址。

```text
symbolTable.txt

1   name    value
2   main    0x84eee8
```

#### 测试用例 2：01.txt（图 3-3）

如图 3-3 所示，全部的 KW、IDN、OP、INT 和 SE 识别成功。

```text
01_lexical.txt

1   int     <KW>
2   a       <IDN>
3   =       <OP>
4   3       <INT>
5   ;       <SE>
6   int     <KW>
7   b       <IDN>
8   =       <OP>
9   5       <INT>
10  ;       <SE>
11  void    <KW>
12  main    <IDN>
13  (       <SE>
14  )       <SE>
15  {       <SE>
16  int     <KW>
17  a       <IDN>
18  =       <OP>
19  5       <INT>
20  ;       <SE>
21  return  <KW>
22  a       <IDN>
23  +       <OP>
24  b       <IDN>
25  ;       <SE>
26  }       <SE>
```

输出的符号表如图 3-4 所示。

```text
symbolTable.txt

1   name    value
2   main    0x11bef68
3   b       0x11bef40
4   a       0x11beee8
```

#### 测试用例 3：02.txt（图 3-5）

如图 3-5 所示，全部的 KW、IDN、OP、INT 和 SE 识别成功。

```text
02_lexical.txt

1   void    <KW>
2   main    <IDN>
3   (       <SE>
4   )       <SE>
5   {       <SE>
6   int     <KW>
7   a       <IDN>
8   ,       <SE>
9   b0      <IDN>
10  ,       <SE>
11  _c      <IDN>
12  ;       <SE>
13  a       <IDN>
14  =       <OP>
15  1       <INT>
16  ;       <SE>
17  b0      <IDN>
18  =       <OP>
19  2       <INT>
20  ;       <SE>
21  _c      <IDN>
22  =       <OP>
23  3       <INT>
24  ;       <SE>
25  return  <KW>
26  b0      <IDN>
27  +       <OP>
28  _c      <IDN>
29  ;       <SE>
30  }       <SE>
```

输出的符号表如图 3-6 所示。由此可见，符号表功能实现正确。在第一列中，输出了 IDN 的名称；在第二列中，输出了它的地址。除了 main 外，分别为：

* _c: 0x1eef90
* b0: 0x1eef68
* a: 0x1eef40

```text
symbolTable.txt

1   name    value
2   _c      0x1eef90
3   b0      0x1eef68
4   a       0x1eef40
5   main    0x1eeee8
```

#### 测试用例 4：07.txt（图 3-7）

如图 3-7：

* 全部的 KW、IDN、OP、INT 和 SE 识别成功；
* 特殊的符号如 const 等也可识别；
* 长度大于 1 的数字可以识别；
* 长度大于 1 的标识符可以识别。

```text
07_lexical.txt

1   void    <KW>
2   main    <IDN>
3   (       <SE>
4   )       <SE>
5   {       <SE>
6   const   <KW>
7   int     <KW>
8   a       <IDN>
9   =       <OP>
10  10      <INT>
11  ;       <SE>
12  b       <IDN>
13  =       <OP>
14  5       <INT>
15  ;       <SE>
16  return  <KW>
17  b       <IDN>
18  ;       <SE>
19  }       <SE>
```

输出的符号表如图 3-8 所示。由此可见，符号表功能实现正确。在第一列中，输出了 IDN 的名称；在第二列中，输出了它的地址。除了 main 外，分别为：

* b: 0xf7ef68
* a: 0xf7ef40

```text
symbolTable.txt

1   name    value
2   a       0xf7ef40
3   b       0xf7ef68
4   main    0xf7eee8
```

### 3.2.2 错误样例测试

对于给出的 2 个错误样例，测试输出如下。

#### 测试用例 5：08.txt（图 3-9）

如图 3-9 所示：

* 全部的 KW、IDN、OP、INT 和 SE 识别成功。
* 但是，在第 6 行少了定义变量的 IDN。

```text
08_lexical.txt

1   int     <KW>
2   a       <IDN>
3   =       <OP>
4   3       <INT>
5   ,       <SE>
6   =       <OP>
7   1       <INT>
8   ,       <SE>
9   c       <IDN>
10  =       <OP>
11  3       <INT>
12  ;       <SE>
13  void    <KW>
14  func    <IDN>
15  (       <SE>
16  )       <SE>
17  {       <SE>
18  int     <KW>
19  a       <IDN>
20  =       <OP>
21  5       <INT>
22  ;       <SE>
23  return  <KW>
24  a       <IDN>
25  +       <OP>
26  b       <IDN>
27  ;       <SE>
28  }       <SE>
```

输出的符号表如图 3-10 所示。

```text
symbolTable.txt

1   name    value
2   b       0xfee f90
3   func    0xfeef68
4   c       0xfeef40
5   a       0xfeeee8
```

#### 测试用例 6：10.txt（图 3-11）

```text
10_lexical.txt

1   int     <KW>
2   a       <IDN>
3   =       <OP>
4   10      <INT>
5   void    <KW>
6   main    <IDN>
7   (       <SE>
8   )       <SE>
9   {       <SE>
10  int     <KW>
11  b       <IDN>
12  b       <IDN>
13  =       <OP>
14  2       <INT>
15  return  <KW>
16  b       <IDN>
17  -       <OP>
18  a       <IDN>
19  }       <SE>
```

如图 3-11 所示，全部 token 的 tokenCode 已经成功识别。但是，在部分标识符后存在 SE 未定义。

输出的符号表如图 3-12 所示。

```text
symbolTable.txt

1   name    value
2   b       0x109ef68
3   main    0x109ef40
4   a       0x109eee8
```

### 3.2.3 自动机算法实现测试

下面，将对 NFA、DFA 和 miniDFA 进行打印，可以对比实现成果和理论模型是否一致，从而最直观地判断算法实现的准确性。

#### 1. NFA 测试

使用 `printFSM` 方法对 `createNFA` 后的 NFA 进行输出，结果如图 3-13 所示：

```text
7

========================================================
Node : 0
Input Letters -> 4
Input Letters -> 5
Input Number 0 -> 1
Input Number 1-9 -> 1
Input Number 1-9 -> 2
========================================================
Node : 1
END STATE!
========================================================
Node : 2
Input Number 0 -> 2
Input Number 0 -> 3
Input Number 1-9 -> 2
Input Number 1-9 -> 3
========================================================
Node : 3
END STATE!
========================================================
Node : 4
END STATE!
========================================================
Node : 5
Input Letters -> 5
Input Letters -> 6
Input Number 0 -> 5
Input Number 0 -> 6
Input Number 1-9 -> 5
Input Number 1-9 -> 6
========================================================
Node : 6
END STATE!
========================================================
```

可见，NFA 共 7 个节点，各自的转换规则和上一章描述的 NFA 理论模型（图 2-2）一致。

#### 2. DFA 测试

下面对确定化后的 DFA 进行输出，如图 3-14 所示：

```text
6

========================================================
Node : 0
Input Letters -> 3
Input Number 0 -> 1
Input Number 1-9 -> 2
========================================================
Node : 1
END STATE!
========================================================
Node : 2
END STATE!
Input Number 0 -> 5
Input Number 1-9 -> 5
========================================================
Node : 3
END STATE!
Input Letters -> 4
Input Number 0 -> 4
Input Number 1-9 -> 4
========================================================
Node : 4
END STATE!
Input Letters -> 4
Input Number 0 -> 4
Input Number 1-9 -> 4
========================================================
Node : 5
END STATE!
Input Number 0 -> 5
Input Number 1-9 -> 5
========================================================
```

可见，DFA 共 6 个节点，各自的转换规则和上一章描述的 DFA 理论模型（图 2-3）一致。因此，NFA 确定化算法正确实现。

#### 3. minimized DFA 测试

下面对最小化后的 DFA 进行输出，如图 3-15 所示：

```text
4

========================================================
Node : 0
Input Letters -> 3
Input Number 0 -> 1
Input Number 1-9 -> 2
========================================================
Node : 1
END STATE!
========================================================
Node : 2
END STATE!
Input Number 0 -> 2
Input Number 1-9 -> 2
========================================================
Node : 3
END STATE!
Input Letters -> 3
Input Number 0 -> 3
Input Number 1-9 -> 3
========================================================
```

可见，minimized DFA 共 4 个节点，各自的转换规则和上一章描述的最小化 DFA 理论模型（图 2-4）一致。因此，DFA 最小化的算法正确实现。

### 3.2.4 功能的完善性测试

如下面的源代码所示，为一些其他测试用例：

```c
int a = -10;
void main(){
int b = (a == -1) - 10;
int c1 = a % b + b / a;

b = (-1) * b * (-2);
int _b = a-(b-(c1+5)) + (a-(b*5));
b = a + _b;
int _c1 = -1 * a * 51 + a != _b;
return _c1 - 1 + b;
}
```

如图 3-16 所示为第一句的 tokens 输出。可见，负数的功能识别正确，能够将 `-10` 识别为独立的 `INT`。

```text
1   int     <KW>
2   a       <IDN>
3   =       <OP>
4   -10     <INT>
5   ;       <SE>
```

如图 3-17、3-18、3-19 所示为源代码 3-7 行的 tokens 输出。可见，各种运算符（加减乘除、取模、赋值等 OP）、`void` 和 `int` 等关键字等的功能识别正确。

```text
6   void    <KW>
7   main    <IDN>
8   (       <SE>
9   )       <SE>
10  {       <SE>
11  int     <KW>
12  b       <IDN>
13  =       <OP>
14  (       <SE>
15  a       <IDN>
16  ==      <OP>
17  -1      <INT>
18  )       <SE>
19  -       <OP>
20  10      <INT>
21  ;       <SE>
```

```text
22  int     <KW>
23  c1      <IDN>
24  =       <OP>
25  a       <IDN>
26  %       <OP>
27  b       <IDN>
28  +       <OP>
29  b       <IDN>
30  /       <OP>
31  a       <IDN>
32  ;       <SE>
```

```text
33  b       <IDN>
34  =       <OP>
35  (       <SE>
36  -1      <INT>
37  )       <SE>
38  *       <OP>
39  b       <IDN>
40  *       <OP>
41  (       <SE>
42  -2      <INT>
43  )       <SE>
44  ;       <SE>
```

如图 3-20 所示，为其他标识符测试结果。可以看到，包含下划线等形态的 IDN 均可正确识别。该测试中还包括了多重括号的使用。在这种情况下，程序仍能按照正确的运算顺序识别出来。

```text
45  int     <KW>
46  _b      <IDN>
47  =       <OP>
48  a       <IDN>
49  -       <OP>
50  (       <SE>
51  b       <IDN>
52  -       <OP>
53  (       <SE>
54  c1      <IDN>
55  +       <OP>
56  5       <INT>
57  )       <SE>
58  )       <SE>
59  +       <OP>
60  (       <SE>
61  a       <IDN>
62  -       <OP>
63  (       <SE>
64  b       <IDN>
65  *       <OP>
66  5       <INT>
67  )       <SE>
68  )       <SE>
69  ;       <SE>
```

如图 3-21 所示，进一步测试了带下划线 IDN 的运算操作，同时，也对 `!=` 等多字符算符进行了测试。可见测试成功。

```text
70  b       <IDN>
71  =       <OP>
72  a       <IDN>
73  +       <OP>
74  _b      <IDN>
75  ;       <SE>
76  int     <KW>
77  _c1     <IDN>
78  =       <OP>
79  -1      <INT>
80  *       <OP>
81  a       <IDN>
82  *       <OP>
83  51      <INT>
84  +       <OP>
85  a       <IDN>
86  !=      <OP>
87  _b      <IDN>
88  ;       <SE>
```

如图 3-22 所示，测试了 `return` 语句。该测试的 `return` 更加复杂，包含了中间变量的运算结果。可见测试成功。

```text
89  return  <KW>
90  _c1     <IDN>
91  -       <OP>
92  1       <INT>
93  +       <OP>
94  b       <IDN>
95  ;       <SE>
96  }       <SE>
```

如图 3-23 为符号表的输出结果。可见程序完整地打印出了全部 IDN 的符号表 `name-value` 元组。

```text
1   name    value
2   _c1     0x117efb0
3   _b      0x117ef88
4   a       0x117c360
5   main    0x117eed0
6   b       0x117ef38
7   c1      0x117ef60
```

如上为测试样例的符号表输出，通过分析可以发现符号表的输出正确。

## 3.3 语法分析测试

### 3.3.1 First 集的输出

First 集是由语法分析器通过分析提供的文法自动生成，是指文法中每个非终结符号所能推导出的所有可能的终结符号的集合。它是用来进行语法分析的一种重要工具，可以用来判断一个输入串是否符合文法规则。语法分析器生成的 First 集如图 3-24 所示：

```text
FIRST(*) = {}

addExp          { 'IDN' 'INT' }
addExpAtom      { '$' '+' '-' }
argConst        { '$' ',' }
argExp          { '$' 'IDN' 'INT' }
argFunctionF    { '$' ',' }
argFunctionR    { '$' ',' }
argVarDecl      { '$' ',' }
argVarDef       { '$' '=' }
assignExp       { 'IDN' 'INT' }
assignExpAtom   { '$' '=' }
bType           { 'int' }
block           { '{' }
blockItem       { '$' ';' 'IDN' 'INT' 'const' 'int' 'return' '{' }
callFunc        { '$' '(' }
compUnit        { '$' 'const' 'int' 'void' }
constDecl       { 'const' }
constDef        { 'IDN' }
constExp        { 'IDN' 'INT' }
constInitVal    { 'IDN' 'INT' }
decl            { 'const' 'int' }
eqExp           { 'IDN' 'INT' }
eqExpAtom       { '!=' '$' '==' }
exp             { 'IDN' 'INT' }
funcDef         { 'void' }
funcFParam      { 'int' }
funcFParams     { '$' 'int' }
funcRParam      { 'IDN' 'INT' }
funcRParams     { '$' 'IDN' 'INT' }
funcType        { 'void' }
initVal         { 'IDN' 'INT' }
mulExp          { 'IDN' 'INT' }
mulExpAtom      { '$' '%' '*' '/' }
number          { 'INT' }
program         { '$' 'const' 'int' 'void' }
relExp          { 'IDN' 'INT' }
relExpAtom      { '$' '<' '<=' '>' '>=' }
stmt            { ';' 'IDN' 'INT' 'return' '{' }
unaryExp        { 'IDN' 'INT' }
varDecl         { 'int' }
varDef          { 'IDN' }
```

### 3.3.2 Follow 集的输出

Follow 集是由语法分析器通过分析提供的文法自动生成，是指在语法分析中，对于每个非终结符号，其后继终结符号的集合。对于一个非终结符号 Vn，其 Follow 集合包含所有可以紧随 Vn 出现的终结符号。通过 Follow 集，可以确定在某个非终结符号的右侧可能出现的终结符号，从而在语法分析时进行正确的推导和规约。语法分析器生成的 Follow 集如图 3-25 所示。

```text
FOLLOW(*) = {}

addExp          { '!=' ')' ',' ';' '<' '<=' '=' '==' '>' '>=' }
addExpAtom      { '!=' ')' ',' ';' '<' '<=' '=' '==' '>' '>=' }
argConst        { ',' }
argExp          { ',' }
argFunctionF    { ')' }
argFunctionR    { ')' }
argVarDecl      { ';' }
argVarDef       { ',' ';' }
assignExp       { ')' ',' ';' }
assignExpAtom   { ')' ',' ';' }
bType           { 'IDN' }
block           { '#' ';' 'IDN' 'INT' 'const' 'int' 'return' 'void' '{' '}' }
blockItem       { '}' }
callFunc        { '!=' '%' ')' '*' '+' ',' '-' '/' ';' '<' '<=' '=' '==' '>' '>=' }
compUnit        { '#' }
constDecl       { '#' ';' 'IDN' 'INT' 'const' 'int' 'return' 'void' '{' '}' }
constDef        { ',' ';' }
constExp        { ',' ';' }
constInitVal    { ',' ';' }
decl            { '#' ';' 'IDN' 'INT' 'const' 'int' 'return' 'void' '{' '}' }
eqExp           { ')' ',' ';' '=' }
eqExpAtom       { ')' ',' ';' '=' }
exp             { ')' ',' ';' }
funcDef         { '#' 'const' 'int' 'void' }
funcFParam      { ')' ',' }
funcFParams     { ')' }
funcRParam      { ')' ',' }
funcRParams     { ')' }
funcType        { 'IDN' }
initVal         { ',' ';' }
mulExp          { '!=' ')' '+' ',' '-' ';' '<' '<=' '=' '==' '>' '>=' }
mulExpAtom      { '!=' ')' '+' ',' '-' ';' '<' '<=' '=' '==' '>' '>=' }
number          { '!=' '%' ')' '*' '+' ',' '-' '/' ';' '<' '<=' '=' '==' '>' '>=' }
program         { '#' }
relExp          { '!=' ')' ',' ';' '=' '==' }
relExpAtom      { '!=' ')' ',' ';' '=' '==' }
stmt            { ';' 'IDN' 'INT' 'const' 'int' 'return' '{' '}' }
unaryExp        { '!=' '%' ')' '*' '+' ',' '-' '/' ';' '<' '<=' '=' '==' '>' '>=' }
varDecl         { '#' ';' 'IDN' 'INT' 'const' 'int' 'return' 'void' '{' '}' }
varDef          { ',' ';' }
```

### 3.3.3 预测分析表的输出

预测分析表是语法分析器通过 First 集与 Follow 集推导出，是一种用于语法分析的数据结构，它可以帮助编译器或解释器确定输入的程序是否符合语法规则。预测分析表通常是由语法分析器自动生成的，它包含了输入符号和栈顶符号的组合，以及对应的产生式或动作。

此语法分析器的预测分析表如图 3-26 所示（其中 `-1` 表示该位置为空，`99` 表示产生式为空集，其他数字表示为产生式的编号）。

```text
      != # $ % ( ) * + , - / ; < <= = == > >= IDN INT const int return void { }
addExp         -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
addExpAtom     99 -1 -1 -1 -1 99 -1 0 99 1 -1 99 99 99 99 99 99 99 -1 -1 -1 -1 -1 -1 -1 -1
argConst       -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 99 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
argExp         -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 99 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
argFunctionF   -1 -1 -1 -1 -1 99 -1 -1 0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
argFunctionR   -1 -1 -1 -1 -1 99 -1 -1 0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
argVarDecl     -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 99 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
argVarDef      -1 -1 -1 -1 -1 -1 -1 -1 99 -1 -1 99 -1 -1 0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
assignExp      -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
assignExpAtom  -1 -1 -1 -1 -1 99 -1 -1 99 -1 -1 99 -1 -1 0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
bType          -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1 -1
block          -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1
blockItem      -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 -1 -1 -1 -1 -1 -1 1 1 0 0 1 -1 1 99
callFunc       99 -1 -1 99 0 99 99 99 99 99 99 99 99 99 99 99 99 99 -1 -1 -1 -1 -1 -1 -1 -1
compUnit       -1 99 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 1 -1 -1 -1 -1
constDecl      -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1 -1 -1
constDef       -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1 -1 -1 -1 -1
constExp       -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
constInitVal   -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
decl           -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 0 1 -1 -1 -1 -1 -1
eqExp          -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
eqExpAtom      1 -1 -1 -1 -1 99 -1 -1 99 -1 -1 99 0 -1 0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
exp            -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
funcDef        -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1
funcFParam     -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1 -1
funcFParams    -1 -1 -1 -1 -1 99 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1 -1
funcRParam     -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
funcRParams    -1 -1 -1 -1 -1 99 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
funcType       -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1
initVal        -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
mulExp         -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
mulExpAtom     99 -1 -1 2 -1 99 0 99 99 1 99 99 99 99 99 99 99 99 -1 -1 -1 -1 -1 -1 -1 -1
number         -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1 -1 -1 -1
program        -1 99 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 0 -1 -1
relExp         -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 -1 -1 -1 -1
relExpAtom     99 -1 -1 -1 -1 99 -1 -1 99 -1 -1 99 0 2 99 99 1 3 -1 -1 -1 -1 -1 -1 -1 -1
stmt           -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 -1 -1 -1 -1 -1 -1 0 0 -1 -1 3 -1 2 -1
unaryExp       -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 0 -1 -1 -1 -1 -1 -1
varDecl        -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1 -1 -1 -1
varDef         -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 -1 -1 -1 -1 -1 -1 -1
```

### 3.3.4 预测分析结果

语法分析的输入为词法分析器输出的 token 序列（如左侧所示），输出为 LL(1) 语法预测分析过程（如右侧所示），目的是判断该 token 序列是否符合该 LL(1) 文法。

几个测试用例以及输出结果如下所示：

#### 1. 可规范规约的测试用例 1

输入的单词符号序列如下所示。

```text
1   void    <KW>
2   main    <IDN>
3   (       <SE>
4   )       <SE>
5   {       <SE>
6   const   <KW>
7   Int     <KW>
8   a       <IDN>
9   =       <OP>
10  10      <INT>
11  ,       <SE>
12  b       <IDN>
13  =       <OP>
14  5       <INT>
15  ;       <SE>
16  return  <KW>
17  b       <IDN>
18  ;       <SE>
19  }       <SE>
```

输出的移进-规约序列如下。由于篇幅和排版限制，这里以代码段的形式输出关键部分结果。完整的输出文件将随本报告一同上交。

```text
1   program#void    reduction
2   compUnit#void   reduction
3   funcDef#void    reduction
4   funcType#void   reduction
5   void#void       move
6   ⋯⋯
7   ;#;             move
8   blockItem#}     reduction
9   }#}             move
10  compUnit##      reduction
11  EOF#EOF         accept
```

由此可见，最后输出 `EOF` 和 `accept`，移进归约完成，语法分析正确。

#### 2. 可规范规约的测试用例 2

输入的单词符号序列如下所示。

```text
1   void    <KW>
2   main    <IDN>
3   (       <SE>
4   )       <SE>
5   {       <SE>
6   return  <KW>
7   3       <INT>
8   ;       <SE>
9   }       <SE>
```

输出的移进-规约序列如下。由于篇幅和排版限制，这里以代码段的形式输出部分结果。完整的输出文件将随本报告一同上交。

```text
1   program#void    reduction
2   compUnit#void   reduction
3   funcDef#void    reduction
4   funcType#void   reduction
5   void#void       move
6   ⋯⋯
7   ;#;             move
8   blockItem#}     reduction
9   }#}             move
10  compUnit##      reduction
11  EOF#EOF         accept
```

由此可见，最后输出 `EOF` 和 `accept`，移进归约完成，语法分析正确。

#### 3. 可规范规约的测试用例 3

输入的单词符号序列如下所示。

```text
1   int     <KW>
2   a       <IDN>
3   =       <OP>
4   3       <INT>
5   ;       <SE>
6   int     <KW>
7   b       <IDN>
8   =       <OP>
9   5       <INT>
10  ;       <SE>
11  void    <KW>
12  main    <IDN>
13  (       <SE>
14  )       <SE>
15  {       <SE>
16  int     <KW>
17  a       <IDN>
18  =       <OP>
19  5       <INT>
20  ;       <SE>
21  return  <KW>
22  a       <IDN>
23  +       <OP>
24  b       <IDN>
25  ;       <SE>
26  }       <SE>
```

输出的移进-规约序列如下。由于篇幅和排版限制，这里以代码段的形式输出部分结果。完整的输出文件将随本报告一同上交。

```text
1   program#int     reduction
2   compUnit#int    reduction
3   decl#int        reduction
4   varDecl#int     reduction
5   bType#int       reduction
6   ......
......
7   ;#;             move
8   blockItem#}     reduction
9   }#}             move
10  compUnit##      reduction
11  EOF#EOF         accept
```

由此可见，最后输出 `EOF` 和 `accept`，移进归约完成，语法分析正确。

#### 4. 可规范规约的测试用例 4

输入的单词符号序列如下所示。

```text
1   void    <KW>
2   main    <IDN>
3   (       <SE>
4   )       <SE>
5   {       <SE>
6   int     <KW>
7   a       <IDN>
8   ,       <SE>
9   b0      <IDN>
10  ,       <SE>
11  _c      <IDN>
12  ;       <SE>
13  a       <IDN>
14  =       <OP>
15  1       <INT>
16  ;       <SE>
17  b0      <IDN>
18  =       <OP>
19  2       <INT>
20  ;       <SE>
21  _c      <IDN>
22  =       <OP>
23  3       <INT>
24  ;       <SE>
25  return  <KW>
26  b0      <IDN>
27  +       <OP>
28  _c      <IDN>
29  ;       <SE>
30  }       <SE>
```

输出的移进-规约序列如下。由于篇幅和排版限制，这里以代码段的形式输出部分结果。完整的输出文件将随本报告一同上交。

```text
1   program#void    reduction
2   compUnit#void   reduction
3   funcDef#void    reduction
4   funcType#void   reduction
5   void#void       move
6   ......
7   ;#;             move
8   blockItem#}     reduction
9   }#}             move
10  compUnit##      reduction
11  EOF#EOF         accept
```

由此可见，最后输出 `EOF` 和 `accept`，移进归约完成，语法分析正确。

#### 5. 不可规范规约的测试用例

输入的单词符号序列如下所示。

```text
1   int     <KW>
2   a       <IDN>
3   =       <OP>
4   10      <INT>
5   void    <KW>
6   main    <IDN>
7   (       <SE>
8   )       <SE>
9   {       <SE>
10  int     <KW>
11  b       <IDN>
12  b       <IDN>
13  =       <OP>
14  2       <INT>
15  return  <KW>
16  b       <IDN>
17  -       <OP>
18  a       <IDN>
19  }       <SE>
```

输出的完整移进-规约序列如下。由此可见，最后在 `mulExpAtom#` 输出 `error`，移进归约完成，语法分析错误。

```text
1   program#int     reduction
2   compUnit#int    reduction
3   decl#int        reduction
4   varDecl#int     reduction
5   bType#int       reduction
6   int#int         move
7   varDef#IDN      reduction
8   IDN#IDN         move
9   argVarDef#=     reduction
10  =#=             move
11  initVal#INT     reduction
12  exp#INT         reduction
13  assignExp#INT   reduction
14  eqExp#INT       reduction
15  relExp#INT      reduction
16  addExp#INT      reduction
17  mulExp#INT      reduction
18  unaryExp#INT    reduction
19  number#INT      reduction
20  INT#INT         move
21  mulExpAtom#void error
```

综上所述，我们完成了下列测试。分析可知，其功能实现完整、正确。因此，语法分析测试成功完成。

* FIRST 集合的输出；
* FOLLOW 集合的输出；
* 预测分析表的输出；
* 正确测试用例的移进-规约序列的输出；
* 错误测试用例的移进-归约序列的输出。

